#include <RFM69OOK.h>
#include <SPI.h>
#include <RFM69OOKregisters.h>

RFM69OOK radio;

#define baseQ 200

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

void sendKey(uint16_t key) {
  sendByte( uint8_t((key>>8)&0xFF) );
  sendByte( uint8_t(key&0xFF) );
}

uint16_t seq = 0x00;
uint8_t button = 0x0B;  // desarme
uint16_t key;

void loop() {

  // calc the current key
  key =  ((uint16_t(seq))<<8) | button;

  Serial.print("Send seq");
  Serial.println(seq, HEX);
  for (uint8_t i=0; i<52; i++){
  
  sendKey(key);
  
  //bit stop
  send1();
  quarter();
  quarter();
  quarter();
  quarter();
  quarter();
  quarter();
  quarter();
  quarter();

  }
  seq++;
}


/*

Frigo :
00001000 10001011 0
00001000 00001011 0
00001000 01001011 0
00001000 11001011 0

00010100 10010011 0
00010100 00010011 0
00010100 01010011 0
00010100 11010011 0

1:
00110110 10001011 0
00110110 00001011 0
00110110 01001011 0
00110110 11001011 0

11110010 10001101 0
11110010 00001101 0
11110010 01001101 0
11110010 11001101 0

 */
