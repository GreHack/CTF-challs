// Arduino specific memory handling
#include <avr/pgmspace.h>

// lib for native I2C bus
#include <Wire.h>

//lib for PCA9535
#include <FaBoGPIO40_PCA9698-mod.h>
FaBoGPIO40 faboGPIO;

// md5-mod
// piggy moded MD5
#include <md5.h>

// read on EEprom, deviceaddress for 24C64is 0x50
byte readEEPROM(unsigned int eeaddress ) 
{
  byte rdata = 0xFF;
 
  Wire.beginTransmission(0x50);
  Wire.write((int)(eeaddress >> 8));   // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.endTransmission();
 
  Wire.requestFrom(0x50,1);
 
  if (Wire.available()) rdata = Wire.read();
 
  return rdata;
}

// write on EEprom, deviceaddress for 24C64is 0x50
void writeEEPROM(unsigned int eeaddress, byte data ) 
{
  Wire.beginTransmission(0x50);
  Wire.write((int)(eeaddress >> 8));   // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.write(data);
  Wire.endTransmission();
 
  delay(5);
}


void setup() {
//
// hardware and various init:
//

  // init GPIO extender on the SAO board
  faboGPIO.configuration();
  faboGPIO.portWrite(0,0x01);
  faboGPIO.portWrite(1,0x01);
  faboGPIO.portMode(0, OUTPUT);
  faboGPIO.portMode(1, OUTPUT);

  // I2C hardware bus 
  Wire.begin();

  Serial.begin(115200);
  Serial.println("Arduino: USB serial port started");

}

uint8_t key[32];
uint8_t md5modhash[16];
int16_t i,j;
uint8_t result,state;

void loop() {

  // read key     
  // 0xC3,0xDA,0xB1,0x48,0x06,0x5C,0x69,0x1C,0x3E,0x1F,0xA2,0xCE,0x6E,0xFF,0xA1,0xA8,0xCF,0x2D,0x35,0x54,0x87,0xD1,0x10,0xF8,0xDA,0x75,0x74,0x31,0x05,0xE8,0x0F,0x76
  for (i=0;i<32;i++)
    key[i]=readEEPROM(0x40+i);

  // do hash X time
  for (i=0;i<666;i++)
  {
    md5(&md5modhash,&key,32);
    for (j=0;j<16;j++)
    {
      key[j]=md5modhash[j];
      key[j+16]=md5modhash[j];
    }
  }
  // key = 0xF9,0x9F,0x6D,0xB2,0x2C,0x0C,0xB4,0x90,0x3A,0xCB,0x7E,0x65,0x9E,0x50,0xB2,0x65

  // write key. Need to cut a VCC wire onboard to remove 24C write protect pin
  for (i=0;i<16;i++)
  {
    writeEEPROM(0x80+i,key[i]);
    key[i]=md5modhash[i];     // unusefull, but this one is for reversers
  }
  
  // read key
  for (i=0;i<16;i++)
    key[i]=readEEPROM(0x80+i);

  // check for a non-zero key
  result=0;
  for (i=0;i<16;i++)
    result|=key[i];
  
  if (result)
  {
      Serial.println("Key generated with success!");
      while(1)
      {
        for (i=0;i<8;i++)
        {
          faboGPIO.portWrite(0,1<<i);
          faboGPIO.portWrite(1,1<<i);
          delay(40);
        }
      }
  }
  else
    Serial.println("Fail!");
}
