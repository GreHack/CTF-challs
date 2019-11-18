// target: arduino

// lib for bitbang serial port
#include <NeoSWSerial.h>
    NeoSWSerial ss( 13, 2 );  // RX, TX


//lib for PCA9535
#include <FaBoGPIO40_PCA9698-mod.h>
FaBoGPIO40 faboGPIO;

// lib for I2C bus
#include <Wire.h>

// OLED lib
#include <ss_oled.h>

uint8_t attinyState=0xFF;
uint8_t eepromState=0xFF;
uint8_t state=0;

static void handleRxChar( uint8_t c )
{
  if (c == 'O' or c == 'E')
    attinyState=0;  // ATtiny is alive

  if (c =='O')
      eepromState=1;  // 93c46 is alive
    else
      eepromState=0;  // 93c46 is wrong
}

byte readEEPROM(int deviceaddress, unsigned int eeaddress ) 
{
  byte rdata = 0xFF;
 
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8));   // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.endTransmission();
 
  Wire.requestFrom(deviceaddress,1);
 
  if (Wire.available()) rdata = Wire.read();
 
  return rdata;
}

void setup()
{
  ss.attachInterrupt(handleRxChar);
  ss.begin(9600);

  Serial.begin(9600);
  Serial.println("******* Serial port started *******");
  Serial.println();
  
  faboGPIO.configuration();
  faboGPIO.portMode(0, OUTPUT);
  faboGPIO.portMode(1, OUTPUT);

  oledInit(OLED_128x64, 0, 0, -1, -1, 400000L);       // Standard HW I2C bus at 400Khz

  oledFill(0, 1);
}

void loop() {
  delay(500);

  Serial.println("******* Hardware self-testing *******");

  // check all hardware attached to I2C bus
  Wire.beginTransmission(0x20);
  if (!Wire.endTransmission())
  {
    oledWriteString(0, 0, 0,(char *)"SAO:OK", FONT_SMALL, 0, 1);
    Serial.println("SAO:OK");
  }
  else
  {
    oledWriteString(0, 0, 0,(char *)"SAO:HS", FONT_SMALL, 0, 1);
    Serial.println("SAO:HS");
  }

  Wire.beginTransmission(0x3C);
  if (!Wire.endTransmission())
  {
    oledWriteString(0, 40, 0,(char *)"OLED:OK", FONT_SMALL, 0, 1);
    Serial.println("OLED:OK");
  }
  else
  {
    oledWriteString(0, 40, 0,(char *)"OLED:HS", FONT_SMALL, 0, 1);
    Serial.println("OLED:HS");
  }

  Wire.beginTransmission(0x50);
  if (!Wire.endTransmission())
  {
    oledWriteString(0, 86, 0,(char *)"24C:OK", FONT_SMALL, 0, 1);
    Serial.println("24C:OK");
  }
  else
  {
    oledWriteString(0, 86, 0,(char *)"24C:HS", FONT_SMALL, 0, 1);
    Serial.println("24C:HS");
  }


  // check if the seial link beetween ATtiny & Arduino OK (means ATtiny is up)
  if (attinyState!=0xFF)
  {
    oledWriteString(0, 0, 2,(char *)"ATtiny:OK", FONT_SMALL, 0, 1);
    Serial.println("ATtiny:OK");
  }
  else
  {
    oledWriteString(0, 0, 2,(char *)"ATtiny:HS", FONT_SMALL, 0, 1);
    Serial.println("ATtiny:HS");
  }

  if (eepromState==1)
  {
    oledWriteString(0, 58, 2,(char *)"93c+data:OK", FONT_SMALL, 0, 1);
    Serial.println("93c+data:OK");
  }
  else if (eepromState==0)
  {
    oledWriteString(0, 58, 2,(char *)"93c+data:HS", FONT_SMALL, 0, 1);
    Serial.println("93c+data:HS");
  }
  else
  {
    oledWriteString(0, 58, 2,(char *)"93c+data:**", FONT_SMALL, 0, 1);
    Serial.println("93c+data:**");
  }

  uint8_t tab1[16],tab2[16];

  oledWriteString(0, 0, 4,(char *)"24c first 32 bytes:", FONT_SMALL, 0, 1);

  uint8_t i;
  for (i=0;i<16;i++)
    tab1[i]=readEEPROM(0x50,i);
  for (i=0;i<16;i++)
    tab2[i]=readEEPROM(0x50,16+i);
    
  oledWriteString(0, 0, 5,(char *)tab1, FONT_SMALL, 0, 1);
  oledWriteString(0, 0, 6,(char *)tab2, FONT_SMALL, 0, 1);
  Serial.println((char*)tab1);
  Serial.println((char*)tab2);
  Serial.println();

  // test the SAO's 16 LED, blink
  state^=0xFF;
  faboGPIO.configuration();
  faboGPIO.portMode(0, OUTPUT);
  faboGPIO.portMode(1, OUTPUT);
  faboGPIO.portWrite(0,state);
  faboGPIO.portWrite(1,state);

  }
