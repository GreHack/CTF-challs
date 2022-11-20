# Game Over 2

## Information to set up the chall on CTFd

_Category_: Hardware

_Points_: 200 (medium-)

_Flag_: `GH22{GG!DFUwasTHEpathTOfollow}`

## Description to put on CTFd

4 identical devices are located beside the staff's desk.

It's a fully functional device, press A to switch it ON, and B to cut it OFF.

Dump the malware!

Hint: there is no need to open it, the device is in RDP1 mode ;)

Author: `Phil`

## How to access

N/A

## Configuration

N/A

## WU

The device have a bootloader in DFU mode. During 10 seconds it can easily be dump with any tool handling DFU.

Under Linux, dfu-util can be used.

dfu-util -l   --> gives you the bootloader entries

The most important things is to target the first entry, to dump the full flash minus the (unuseful) bootloader

dfu-util -c 1 -i 0 -a 2 -U firmware.bin -Z 57344   --> dump the full firmware

Then "strings firmware.bin | grep GH22"
