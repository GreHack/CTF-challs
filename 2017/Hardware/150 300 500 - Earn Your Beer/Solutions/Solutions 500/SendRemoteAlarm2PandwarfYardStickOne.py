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

# btn 1 : 1 10010110 01001101 00100010 00000101 1	
# btn 2 : 1 00000010 01010000 00100010 01110100 1
# btn 3 : 1 11000000 10000100 00100010 01100110 1
# btn 4 : 1 10110100 00100011 00100010 11111001 1

key = 0x964D2205
binKey = bin(key)
print "Key:",key
print "binKey:",binKey

currentStrKey = "100000" # prembule 1 + 1ms = 100000
strKey = str(binKey)[2:]
for currentBit in strKey:
        if(currentBit == "0"):
                currentStrKey = currentStrKey + "1110" # 150ms up, 50ms down
        if(currentBit == "1"):
                currentStrKey = currentStrKey + "1000" # 50ms up, 150ms down

finalStrKey = currentStrKey + "10000000000"  # leading 

finalStrKey = finalStrKey * 12

finalStrKey = "0b" + finalStrKey


print "Binary (PWM) key: ",finalStrKey
print "Len: ",len(finalStrKey)

finalStrKey = str(finalStrKey)[2:]
readyKey = ''.join(chr(int(finalStrKey[i:i+8], 2)) for i in xrange(0, len(finalStrKey), 8))
keyLen = len(readyKey)

#print "key to send: ",readyKey
ConfigureD(d)

while True:
    print "TX'ing key..."
    d.RFxmit(readyKey)
    raw_input("press enter to send again")
print "Done."
