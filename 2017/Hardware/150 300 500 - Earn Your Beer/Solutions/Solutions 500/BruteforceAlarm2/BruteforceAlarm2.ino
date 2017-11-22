#include <RFM69OOK.h>
#include <SPI.h>
#include <RFM69OOKregisters.h>

RFM69OOK radio;

#define baseQ 500

void setup() {
  Serial.begin(115200);
  Serial.println("Let's GO !");

  Serial.println("Initialize...");
  radio.initialize();

  Serial.println("Begin transmit");
  radio.transmitBegin();

  Serial.println("Set freq");
  radio.setFrequencyMHz(433.92);

  Serial.println("Set power");  // parasite
  radio.setPowerLevel(20);
}

void delayMicros(uint32_t d) {
  uint32_t t = micros() + d;
  while(micros() < t);
}

void quarter(){
  delayMicroseconds(baseQ);
}

void send1() {
  radio.send(1);
  quarter();
  radio.send(0);
  quarter();
  quarter();
  quarter();
}

void send0() {
  radio.send(1);
  quarter();
  quarter();
  quarter();
  radio.send(0);
  quarter();
}

void sendByte(uint8_t data) {
  int8_t i=7;
  for (;i>-1;i--) {
    if( data & (1<<i) )
      send1();
    else
      send0();
  }
}

uint8_t checksumRemote(uint16_t seqNumber, uint8_t button) {
  return (( ((seqNumber>>8)&0xFF)  + (seqNumber&0xFF) + button) & 0xFF);
}

void sendKey(uint32_t key) {
  sendByte( uint8_t((key>>24)&0xFF) );
  sendByte( uint8_t((key>>16)&0xFF) );
  sendByte( uint8_t((key>>8)&0xFF) );
  sendByte( uint8_t(key&0xFF) );
}

uint16_t seq = 0x0000;
uint8_t button = 0x21;
uint32_t key;

void loop() {

  // calc the current key
  key = ((uint32_t(seq))<<16) | ((uint16_t(button))<<8) | checksumRemote(seq,button);

  Serial.print("Send seq");
  Serial.println(seq, HEX);
  for (uint8_t i=0; i<12; i++){
  
    //preambule
    send1();
    quarter();
    quarter();

    sendKey(key);
  
    //bit stop
    send1();

  }
  seq++;
}


/*
2 :
A: 1 10010110 01001101 00100010 00000101 1  964D2205
D: 1 10010110 01001101 00100001 00000100 1  964D2104
P: 1 10010110 01001101 00100100 00000111 1  964D2407
B: 1 10010110 01001101 00101000 00001011 1  964D280B

2 :
A: 1 11000000 10000100 00100010 01100110 1  C0842266
D: 1 11000000 10000100 00100001 01100101 1  C0842165
P: 1 11000000 10000100 00100100 01101000 1  C0842468
B: 1 11000000 10000100 00101000 01101100 1  C084286C

2 :
A: 1 10110100 00100011 00100010 11111001 1  B42322F9
D: 1 10110100 00100011 00100001 11111000 1  B42321F8
P: 1 10110100 00100011 00100100 11111011 1  B42324FB
B: 1 10110100 00100011 00101000 11111111 1  B42328FF

Frigo
A: 1 00000010 01010000 00100010 01110100 1  02502274
D: 1 00000010 01010000 00100001 01110011 1  02502173
P: 1 00000010 01010000 00100100 01110110 1  02502476
B: 1 00000010 01010000 00101000 01111010 1  0250287A


 */
