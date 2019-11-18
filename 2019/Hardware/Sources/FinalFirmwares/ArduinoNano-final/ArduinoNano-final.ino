// define a few things

// pins for the Shitty Add-On extension LVL5 (used for a shortcut to run hidden mode)
#define SAO_IO1 14
#define SAO_IO2 15

// pins for the GPIO for LVL6
#define GPIO0 3
#define GPIO1 4
#define GPIO2 5
#define GPIO3 6
#define GPIO4 7


// Arduino specific memory handling
#include <avr/pgmspace.h>

// lib for bitbang serial port
#include <NeoSWSerial.h>
NeoSWSerial ss( 13, 9 );  // RX, TX

//lib for PCA9535
#include <FaBoGPIO40_PCA9698-mod.h>
FaBoGPIO40 faboGPIO;

// lib for native I2C bus
#include <Wire.h>

// lib for OLED screen
#include <ss_oled-mod.h>

// lib for CRC32 of the ATtiny's HASH 
#include <CRC32.h>

// include main menu picture
#include "LandscapeMain.h"

// include crypted picture for LVL5
#include "GhostWires.h"

// include crypted picture for LVL6
#include "GlassesCrypt.h"

// include crypted picture for LVL7
#include "LlamaCrypt.h"

// include crypted picture for LVL8
#include "WindozCrypt.h"

// include firmware for LVL8
#include "GH19-chall8.ino.bin.7z.bin.h"

// md5-mod
// piggy moded MD5
#include <md5.h>


// global var for interrupt code
uint8_t currentHash[16];
uint8_t state=0;
uint8_t count=0;
uint32_t crc32=0;
CRC32 crc;


// callback on each byte received by the software serial port interupt
static void handleRxChar( uint8_t c )
{
  // frame format from ATtiny:
  // :hash:XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX:CRLF
  // HASH with 32 bytes from the 93C46:
  // :hash:24F126ABFD27FD847ACA02D4B64A868D: (CRC32=6BE4A0B4)
  // HASH with 64 bytes from the 93C46:
  // :hash:49D5B5A8FBB6D02894F0276198A9BDDB: (CRC32=7257AF43)


  // 0: wait for LF -> then count=0, state=1
  // 1: skip 5, count++, if 5 count=0 state=2
  // 2: read hash

  //Serial.write(c); // Debug things caming from ATtiny
  switch (state)
  {
    
    case 0: // seek for \n
      if (c=='\n')
        state=1;
      break;

    case 1: // skip ":hash" and check if the next char is ':'
      if (count==5)
      {
        count=0;
        if (c==':')
          state=2;
        else
          state=0;
      }
      else
        count++;
      break;

    case 2: // read the hash and do a string to hex convertion
      if ( (c>='0' && c<='9') || (c>='A' && c<='F') )
      {
        if (c>='0' && c<='9')
          c=c-'0';
        else
          c=c-'A'+10;

        if (count%2==0)
          currentHash[count/2]=c<<4;
        else
          currentHash[count/2]=currentHash[count/2]|c;

        if (count==31)
        {
          // calc CRC32
          crc32 = CRC32::calculate(currentHash, 16);
          count=0;
          state=0;
        }
        count++;          
      }
      else
      {
        count=0;
        state=0;
        crc32=0;
      }
      break;
  }
 
}


// read on EEprom, deviceaddress for 24C64 is 0x50
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


// animate LED, pattern 1 (line), deadloop
uint8_t eyesState=0;
uint32_t eyesCount=0;
#define eyesWait  1
#define eyesWait2 100000
void animateLED1()
{
        while(1)
        {
          switch(eyesState)
          {
            case 0:
              if (eyesCount==0)
              {
                faboGPIO.portWrite(0,0x01);
                faboGPIO.portWrite(1,0x01);
                eyesCount++;
              }
              else if (eyesCount<eyesWait2)
              {
                eyesCount++;
              }
              else
              {
                eyesCount=0;
                eyesState=1;
              }
              break;
              
            case 1:
              if (eyesCount==0)
              {
                faboGPIO.portWrite(0,0x82);
                faboGPIO.portWrite(1,0x82);
                eyesCount++;
              }
              else if (eyesCount<eyesWait2)
              {
                eyesCount++;
              }
              else
              {
                eyesCount=0;
                eyesState=2;
              }
              break;
              
            case 2:
              if (eyesCount==0)
              {
                faboGPIO.portWrite(0,0x44);
                faboGPIO.portWrite(1,0x44);
                eyesCount++;
              }
              else if (eyesCount<eyesWait2)
              {
                eyesCount++;
              }
              else
              {
                eyesCount=0;
                eyesState=3;
              }
              break;
              
            case 3:
              if (eyesCount==0)
              {
                faboGPIO.portWrite(0,0x28);
                faboGPIO.portWrite(1,0x28);
                eyesCount++;
              }
              else if (eyesCount<eyesWait2)
              {
                eyesCount++;
              }
              else
              {
                eyesCount=0;
                eyesState=4;
              }
              break;
              
            case 4:
              if (eyesCount==0)
              {
                faboGPIO.portWrite(0,0x10);
                faboGPIO.portWrite(1,0x10);
                eyesCount++;
              }
              else if (eyesCount<eyesWait2)
              {
                eyesCount++;
              }
              else
              {
                eyesCount=0;
                eyesState=0;
              }
              break;
              
          }
        }
}


// blink eyes, no dedloop
void animateLED2()
{
      switch(eyesState)
      {
          case 0:
            if (eyesCount==0)
            {
              faboGPIO.portWrite(0,0x00);
              faboGPIO.portWrite(1,0x00);
              eyesCount++;
            }
            else if (eyesCount < 100)
            {
              eyesCount++;
              break;
            }
            else
            {
              if ((uint8_t)random(0,200)==0)
              {
                eyesCount=0;
                eyesState=1;
              }
            }
            break;

          case 1:
            if (eyesCount==0)
            {
              faboGPIO.portWrite(0,0x07);
              faboGPIO.portWrite(1,0x07);
              eyesCount++;
            }
            else if (eyesCount<eyesWait)
            {
              eyesCount++;
            }
            else
            {
              eyesCount=0;
              eyesState=2;
            }
            break;
            
          case 2:
            if (eyesCount==0)
            {
              faboGPIO.portWrite(0,0x8F);
              faboGPIO.portWrite(1,0x8F);
              eyesCount++;
            }
            else if (eyesCount<eyesWait)
            {
              eyesCount++;
            }
            else
            {
              eyesCount=0;
              eyesState=3;
            }
            break;
            
          case 3:
            if (eyesCount==0)
            {
              faboGPIO.portWrite(0,0xFF);
              faboGPIO.portWrite(1,0xFF);
              eyesCount++;
            }
            else if (eyesCount<eyesWait)
            {
              eyesCount++;
            }
            else
            {
              eyesCount=0;
              eyesState=4;
            }
            break;

          case 4:
            if (eyesCount==0)
            {
              faboGPIO.portWrite(0,0x8F);
              faboGPIO.portWrite(1,0x8F);
              eyesCount++;
            }
            else if (eyesCount<eyesWait)
            {
              eyesCount++;
            }
            else
            {
              eyesCount=0;
              eyesState=5;
            }
            break;
            
          case 5:
            if (eyesCount==0)
            {
              faboGPIO.portWrite(0,0x07);
              faboGPIO.portWrite(1,0x07);
              eyesCount++;
            }
            else if (eyesCount<eyesWait)
            {
              eyesCount++;
            }
            else
            {
              eyesCount=0;
              eyesState=0;
            }
            break;
      }
}

// check if 2 I/O pin of the SAO connector is shorted. ret True if YES.
// code to reverse to win LVL5
uint8_t checkSAOwire()
{
  pinMode(SAO_IO1, OUTPUT);
  pinMode(SAO_IO2, INPUT);
  delay(1);
  digitalWrite(SAO_IO1, 1);   // write 1 on SAO pin extension
  delay(1);
  if (digitalRead(SAO_IO2)==0)
    return 0;
  digitalWrite(SAO_IO1, 0);   // write 0 on SAO pin extension
  delay(1);
  if (digitalRead(SAO_IO2)==1)
    return 0;
  return 1;   // 2 tests passed, means 2 pins are shorted, you win (challenge 5).
}


// code for LVL5
uint8_t checkSAOshortcut()
{
  //check for shortcut on SAO pin
  oledWriteString(0, 0, 1,(char *)"Hiddenmode:", FONT_SMALL, 0, 1,1);
  if (checkSAOwire())   // patch this call or reverse inside to solve challenge
  {
    /* cypher code to generate the flag, (humhum...)
    // for information only
    for (i=0;i<sizeof(message);i++)
    {
      switch (i%4)
      {
        case 0:
          flagLVL5[i]+=(uint8_t)((int)key[i%4]-49);
          break;
        case 1:
          flagLVL5[i]-=(uint8_t)((int)key[i%4]+23);
          break;
        case 2:
          flagLVL5[i]^=key[i%4];
          break;
        case 3:
          flagLVL5[i]-=(uint8_t)((int)key[i%4]+(int)key[(i%4)-1]);
          break;
      }
    } */

    // display flag
    uint8_t key[4];   // from 24C64 offset 0x52{0x35, 0x54, 0x87, 0xD1}
    // GH19{!GoodShortCut!}
    unsigned char flagLVL5[136] = {
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x4E, 0x69, 0x63, 0x65, 0x21, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x54, 0x68, 0x69, 0x73,
    0x20, 0x6C, 0x69, 0x74, 0x74, 0x6C, 0x65, 0x20, 0x77, 0x69, 0x72, 0x65,
    0x20, 0x77, 0x65, 0x6C, 0x6C, 0x20, 0x70, 0x6C, 0x61, 0x63, 0x65, 0x64,
    0x20, 0x67, 0x69, 0x76, 0x65, 0x73, 0x20, 0x79, 0x6F, 0x75, 0x20, 0x61,
    0x20, 0x66, 0x65, 0x77, 0x20, 0x70, 0x6F, 0x69, 0x6E, 0x74, 0x73, 0x20,
    0x3A, 0x29, 0x2E, 0x20, 0x48, 0x65, 0x72, 0x65, 0x20, 0x69, 0x73, 0x20,
    0x74, 0x68, 0x65, 0x20, 0x66, 0x6C, 0x61, 0x67, 0x3A, 0x20, 0x4B, 0xDD,
    0xB6, 0xE1, 0x7F, 0xB6, 0xC0, 0x17, 0x73, 0xF9, 0xD4, 0x10, 0x73, 0x07,
    0xF3, 0xEB, 0x79, 0x09, 0xA6, 0x25, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20 };

    int16_t i;
    uint8_t *ptrChar=&flagLVL5[0x6A];

    // read key from EEprom to complexify static reverse
    // without this key, impossible to compute the result
    // reverse to get the flag cost more than the intented path ;)
    for (i=0;i<4;i++)
      key[i]=readEEPROM(0x52+i);

    // uncrypt the flag
    for (i=0;i<20;i++)
    {
      switch (i%4)
      {
        case 0:
          ptrChar[i]-=(uint8_t)(key[0]-49);
          break;
        case 1:
          ptrChar[i]+=(uint8_t)(key[1]+23);
          break;
        case 2:
          ptrChar[i]^=key[2];
          break;
        case 3:
          ptrChar[i]+=(uint8_t)(key[3]+key[(3)-1]);
          break;
      }
    }

    for(i=0;i<10;i++)
    {
      oledWriteString(0, 70, 1,(char *)"**Found**" , FONT_SMALL, 0, 1,1);
      delay(300);    
      oledWriteString(0, 70, 1,(char *)"**Found**" , FONT_SMALL, 1, 1,1);
      delay(300);    
    }

    oledLoadBMP(GhostWires,0,1,NULL);
    while(1)
    {
      for(i=0;i<1016;i++)
      {
        //oledWriteString(i, 60, 4,(char *)flagLVL5, FONT_STRETCHED, 1, 1,1);
        oledWriteString(i, 60, 4,(char *)flagLVL5, FONT_NORMAL, 1, 1,1);
        //delay(20);
      }
    }
  }
  else
  {
    pinMode(SAO_IO1, INPUT);
    oledWriteString(0, 66, 1,(char *)"Not Found" , FONT_SMALL, 0, 1,1);
    // continu
  }
}


// check if the SAO is connected
uint8_t checkSAO()
{
  Wire.beginTransmission(0x20);   // I2C slave address of the PCA9535
  return Wire.endTransmission();
}


// check pins state for the GPAO 
// each time something is wrong, return 0; to generate a more simple code to reverse
// return 0 = #fail
// return value = good GPIO state to uncrypt flag
uint8_t checkGPIOpins()
{
  uint8_t i,j;

  pinMode(GPIO0, INPUT);
  pinMode(GPIO1, INPUT);
  pinMode(GPIO2, OUTPUT);
  pinMode(GPIO3, INPUT);
  pinMode(GPIO4, INPUT);
  delay(1);
  
  for (j=0;i<2;j++) // pass number 0, 1 and 2
  {
    for (i=0;i<4;i++) // check 0, 1, 2 and 3
    {
      switch(i)
      {
        case 0:   // GPIO0 = 1
          if (digitalRead(GPIO0)==0)
            return 0;
          delay(1);
        break;
  
        case 1:   // GPIO1 = 0
          if (digitalRead(GPIO1)==1)
            return 0;
          delay(1);
        break;

        case 2:   // GPIO3 = GPIO2 (pass 0=1, pass 1=0, pass 2=1)
          switch (j)
          {
            case 0:
              digitalWrite(GPIO2, 1);     // write 1 on GPIO2
              delay(1);
              if (digitalRead(GPIO3)==0)  // must be 1 on GPIO3
                return 0;
            break;

            case 1:
              digitalWrite(GPIO2, 0);     // write 0 on GPIO2
              delay(1);
              if (digitalRead(GPIO3)==1)  // must be 0 on GPIO3
                return 0;
            break;

            case 2:
              digitalWrite(GPIO2, 1);     // write 1 on GPIO2
              delay(1);
              if (digitalRead(GPIO3)==0)  // must be 1 on GPIO3
                return 0;
            break;
          }
        break;

        case 3:   // GPIO4 = 1
          if (digitalRead(GPIO4)==0)
            return 0;
          delay(1);
        break;
      }
    }
    return (PIND&B11111000);    // return the state of the 5 GPIO pin, must be B11101000
  }
}

// code for LVL6, check for GPIO pins states & display flag if OK
uint8_t GPAOcount=0;
void checkGPIO()
{

// buffer for hash
uint8_t md5hash[16];
uint8_t result;

  result=checkGPIOpins();
  if (result)
    GPAOcount++;
  else
    GPAOcount=0;
    
  if (result && GPAOcount>50)  
  {
    md5(&md5hash,&result,1);  // hash only byte 0xE8, value of the GPIO portD = {0x12, 0x99, 0xC4, 0x6B, 0x5C, 0x27, 0x76, 0x4E, 0x64, 0xF2, 0x03, 0x21, 0x41, 0x17, 0x7F, 0x2E}
    oledLoadBMP(GlassesCrypt,0,1,md5hash);  // display BMP with glasses 
    while(1)                                // randomly blink LED
    {
      faboGPIO.portWrite(0,random(0,0xFF));
      faboGPIO.portWrite(1,random(0,0xFF));
      delay(100);
    }
  }
}

void setup() {

//
// hardware and various init:
//

  // onboard USB<->Serial port
  Serial.begin(115200);
  Serial.println("Arduino: USB serial port started");

  // serial port, ATtiny->Arduino
  ss.attachInterrupt(handleRxChar);
  ss.begin(9600);
  ss.println("Arduino: software serial port started");

  // I2C hardware bus 
  Wire.begin();

  // screen init
  oledInit(OLED_128x64, 0, 0, -1, -1, 400000L);
  oledFill(0, 1); // clear

  // init GPIO extender on the SAO board
  faboGPIO.configuration();
  faboGPIO.portWrite(0,0x00);   // clear port BEFORE to avoid LED's glitch at power-up
  faboGPIO.portWrite(1,0x00);   // 
  faboGPIO.portMode(0, OUTPUT);
  faboGPIO.portMode(1, OUTPUT);

//
// boot
//

  // start string fake boot delay for fun :)
  oledWriteString(0, 0, 0,(char *)"Hardware starting", FONT_SMALL, 0, 1,1);
  delay(1000);
  oledWriteString(0, 102, 0,(char *)".", FONT_SMALL, 0, 1,1);
  delay(1000);
  oledWriteString(0, 106, 0,(char *)".", FONT_SMALL, 0, 1,1);
  delay(1000);
  oledWriteString(0, 110, 0,(char *)".", FONT_SMALL, 0, 1,1);
  delay(2000);

  // check for shortcut on the SAO connector
  checkSAOshortcut();

  oledWriteString(0, 0, 2,(char *)"Check SAO:", FONT_SMALL, 0, 1,1);
  if (checkSAO())
  {
    // not connected
    oledWriteString(0, 60, 2,(char *)"Not found", FONT_SMALL, 0, 1,1);
    oledWriteString(0, 48, 5,(char *)"STOP", FONT_NORMAL, 0, 1,1); // TODO try a sprite like moving here
    while(1){}
  }
  else
  {
    // connected
    oledWriteString(0, 60, 2,(char *)"Found", FONT_SMALL, 0, 1,1);
    oledWriteString(0, 0, 4,(char *)"Board is ready,", FONT_SMALL, 0, 1,1);
    oledWriteString(0, 93, 4,(char *)"great.", FONT_SMALL, 0, 1,1);
    oledWriteString(0, 0, 5,(char *)"Flag is:", FONT_SMALL, 0, 1,1);
    oledWriteString(0, 0, 7,(char *)"GH19{NoWhiteSmokeYet}", FONT_SMALL, 0, 1,1);
    delay(10000);
  }
  
}

// code to uncompress LUT table from 24C EEprom + 11 bits to 8 bits
int32_t getValue(uint32_t *a, int32_t bits, int32_t i) {
    int32_t bitoffset = i * bits;
    int32_t index = bitoffset / 32;
    int32_t shift = bitoffset % 32;
    uint64_t maskbits = (~(uint64_t)0) >> (64-bits);
    int32_t val = ((((uint64_t)pgm_read_dword(&a[index+1])<<32) + pgm_read_dword(&a[index])) >> shift) & maskbits;
    return(val);
}

uint8_t LVL8key[16];
void LVL8code()
{
  uint8_t i,result=0;

  for(i=0;i<16;i++)
    result|=LVL8key[i];
  if (result)
  {
    oledLoadBMP(WindozCrypt,0,1,LVL8key);

    while(1)
    {
        for (i=0;i<8;i++)
        {
          faboGPIO.portWrite(0,1<<(7-i));
          faboGPIO.portWrite(1,1<<(7-i));
          delay(25);
        }

    }
  }
}


void mainLoop()
{
//  Main:
//  
//  read 24C64 flag challenge 2
//  check GPIO for LVL6 -> display crypted image with ATtiny checksum
//  check CRC32 and display flag/screen LVL7

uint8_t j;
uint8_t LVL7key[16]; //crc32+currentHash[4]..currentHash[15]
uint32_t *ptr32;

      // read and discard flag LVL2, need to be intercepted with logical analyser
      for (j=0;j<32;j++)
        readEEPROM(j);

      // read 16 bytes for LVL8 key
      for (j=0;j<16;j++)
        LVL8key[j]=readEEPROM(0x80+j);

      // check GPIO for LVL6 and display flag if correct states are found
      checkGPIO();

      if (crc32 == 0x7257AF43)
      {
        memcpy(LVL7key,currentHash,16);   // 49D5B5A8FBB6D02894F0276198A9BDDB
        ptr32=(uint32_t*)LVL7key;
        *ptr32=crc32;                    // crc32=7257AF43 -> 43AF5772FBB6D02894F0276198A9BDDB
        
        oledLoadBMP(LlamaCrypt,0,1,LVL7key);

        faboGPIO.portWrite(0,0x00);
        faboGPIO.portWrite(1,0x00);
        eyesState=0;
        eyesCount=0;

        animateLED1();
      }

      animateLED2();  // randomly blink eyes

      LVL8code();     // call display code for LVL8
}

void scrollText(const uint8_t* PROGMEM ptr,uint16_t count)
{
    oledWriteString(count, 0, 7,ptr, FONT_NORMAL, 0, 1,0);
}

// main scrolltext len=905 chars
//const uint8_t txtMain[] PROGMEM = "                            Hi! So you've boot the board and all seems fine, great!                 Now, a little word about the project. This series of hardware challenges are mostly designed for people beginning in hardware hacking. If you're stuck somewhere, do not hesitate to catch someone from the GreHack's staff and ask for explanations. We hope you'll get some fun while trying to get all the flag out of this board.                About this scroll-text, in the pure tradition of the demo-scene, the scroll-text is VERY important. And the most awaited part is the greetings. Here we go: greetz to all people who've made this hardware possible, Joe Grand, Baldanos, Marc Olanie and last but not least Mr. Optimization.             Now another important thing: at the end of the scroll-text something new happened to this board, just before it start again, it's important for challenge 8. It's NOW!";
const uint8_t txtMain0[] PROGMEM = "                            Hi! So you've boot the board and all seems fine, great!                ";
const uint8_t txtMain1[] PROGMEM = "                Now, a little word about the project. This series of hardware challenges are mostly designed for people beginning in hardware hacking.                ";
const uint8_t txtMain2[] PROGMEM = "                If you're stuck somewhere, do not hesitate to catch someone from the GreHack's staff and ask for explanations.                ";
const uint8_t txtMain3[] PROGMEM = "                We hope you'll get some fun while trying to get all the flags out of this board.                ";
const uint8_t txtMain4[] PROGMEM = "                About this scroll-text, in the pure tradition of the demo-scene, the scroll-text is VERY important. And the most awaited part is the greetings.                ";
const uint8_t txtMain5[] PROGMEM = "                Here we go: greetz to all people who've made this hardware possible, Joe Grand, Baldanos, Marc Olanie and last but not least Mr. Optimization.                ";
const uint8_t txtMain6[] PROGMEM = "                Now another important thing: at the end of the scroll-text something new happens to this board, just before it start again, it's important for challenge 8. It's NOW!               ";

void loop() {

uint16_t i,count=0;
uint8_t state=0;


  faboGPIO.portWrite(0,0x00);
  faboGPIO.portWrite(1,0x00);

  oledLoadBMP(landscapeMain,0,1,NULL);

  while(1)
  {
      // do mandatory mainLoop stuff
      mainLoop();

      switch(state)
      {
          case 0:
            if (count>792)
            {
              state=1;
              count=0;
            }
            else
            {
              //oledWriteString(count, 0, 7,txtMain0, FONT_NORMAL, 0, 1,0);
              scrollText(txtMain0,count);
              count++;
            }
            delay(10);
            break;
            
          case 1:
            if (count>1328)
            {
              state=2;
              count=0;
              // add hint to LVL8, should help people to keep an eye on the serial port ;)
              Serial.println("Nothing to read here...");
            }
            else
            {
              //oledWriteString(count, 0, 7,txtMain1, FONT_NORMAL, 0, 1,0);
              scrollText(txtMain1,count);
              count++;
            }
            delay(6);
            break;

          case 2:
            if (count>1136)
            {
              state=3;
              count=0;
            }
            else
            {
              //oledWriteString(count, 0, 7,txtMain2, FONT_NORMAL, 0, 1,0);
              scrollText(txtMain2,count);
              count++;
            }
            delay(8);
            break;

          case 3:
            if (count>816)
            {
              state=4;
              count=0;
            }
            else
            {
              //oledWriteString(count, 0, 7,txtMain3, FONT_NORMAL, 0, 1,0);
              scrollText(txtMain3,count);
              count++;
            }
            delay(10);
            break;

          case 4:
            if (count>1400)
            {
              state=5;
              count=0;
            }
            else
            {
              //oledWriteString(count, 0, 7,txtMain4, FONT_NORMAL, 0, 1,0);
              scrollText(txtMain4,count);
              count++;
            }
            delay(6);
            break;

          case 5:
            if (count>1392)
            {
              state=6;
              count=0;
            }
            else
            {
              //oledWriteString(count, 0, 7,txtMain5, FONT_NORMAL, 0, 1,0);
              scrollText(txtMain5,count);
              count++;
            }
            delay(6);
            break;

          case 6:
            if (count>1576)
            {
              state=42;
              count=0;
            }
            else
            {
              //oledWriteString(count, 0, 7,txtMain6, FONT_NORMAL, 0, 1,0);
              scrollText(txtMain6,count);
              count++;
            }
            delay(5);
            break;

            case 42:
            // LVL8: remove obfuscation with getValue() & send firmware using internal USB serial port.
              for (i=0;i<4139;i++)  // len of the uncompressed firmware for LVL8
                Serial.write(readEEPROM((uint16_t)getValue((uint32_t*)compress,11,i)+6812));
              state=0;

      }
      // end while(1) / main loop()
  }
  // unreachable
}
