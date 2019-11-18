// target: ATtiny85

//lib for bitbang serial output:
#include <SoftwareSerial.h>
// init the serial port
SoftwareSerial mySerial(PB5, PB2); // RX, TX   !!!RX is not used, mapped on RESET


//lib for handling the Microwire EEprom 93xx
#include <MicrowireEEPROM.h>
// Microwire needs four wires (apart from VCC/GND) DO,DI,CS,CLK
// configure them here, note that DO and DI are the pins of the
// EEPROM, so DI is an output of the uC, while DO is an input
//int CS=13; int CLK=12; int DI=7; int DO=2;
int CS=PB0; int CLK=PB3; int DI=PB1; int DO=PB4;

// EEPROMS have different sizes. Also the number of bits per page varies.
// We need to configure the page size in bits (PGS) and address bus width
// in bits (ADW). The speed at which the clock is run is configured in
// microseconds.
int PGS=16; int ADW=6; int SPD=200;

// init the library
MicrowireEEPROM ME(CS, CLK, DI, DO, PGS, ADW, SPD); 

// md5-mod
// piggy moded MD5 to not get the challenge trivial by bypass the code patching to read and hash more bytes.
#include <md5.h>

// buffer for EEprom datas & hash
uint8_t datas[64];
uint8_t md5modhash[16];


// send hex byte in ASCII char
void sendByte(uint8_t byteToA)
{
  mySerial.print(byteToA>>4&0xF,HEX);
  mySerial.print(byteToA&0xF,HEX);
}

void setup() {
    mySerial.begin(9600);
}

void loop() {
    uint8_t msgLen,i;  // need to patched to 64 for level 7

    //msgLen=32;
    
    //for (i=0; i<msgLen ; i++)
    for (i=0; i<32 ; i++)
    {
      datas[i]=(uint8_t)ME.read(i)&0xFF;  // keep LSB only as MSB is always 0x00 because of the 16 bits mode of the 93C46
      delay(15);
    }

    //md5(&md5modhash,datas,msgLen);
    md5(&md5modhash,datas,32);
    
    mySerial.print(":");
    mySerial.print("hash");
    mySerial.print(":");
    for (i=0;i<16;i++)
      sendByte(md5modhash[i]);
    mySerial.println(":");

    delay(1000);

}
