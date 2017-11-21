#include <RFM69OOK.h>
#include <SPI.h>
#include <RFM69OOKregisters.h>

RFM69OOK radio;

#define baseQ 500
uint16_t seq = 0x0250;
uint8_t button = 0x21;
uint32_t key;

void setup() {
  Serial.begin(115200);

  Serial.println("Initialize");
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

void loop() {

  key = ((uint32_t(seq))<<16) | ((uint16_t(button))<<8) | checksumRemote(seq,button);

  for (uint8_t i=0; i<10; i++){

    Serial.print("Send key");
    Serial.println(key, HEX);
  
    //preambule
    send1();
    quarter();
    quarter();

    sendKey(key);
  
    //bit stop
    send1();

  }
  Serial.println("Wait for 5 seconds\n");
  delay(3000);

}

