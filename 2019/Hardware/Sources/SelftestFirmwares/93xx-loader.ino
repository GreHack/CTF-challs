// Target: ATtiny85

//lib for bitbang serial output:
#include <SoftwareSerial.h>

//lib for handling the Microwire EEprom 93xx
#include <MicrowireEEPROM.h>

// init the serial port
SoftwareSerial mySerial(PB5, PB2); // RX, TX   !!!RX is not used, mapped on RESET

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

// 93xx is configured in 16 bits mode, 64 words
uint16_t datas[64]={'H','e','r','e',' ','i','s',' ','a',' ','f','l','a','g',' ','G','H','1','9','{','U','n','k','n','o','w','n','C','h','i','p','}','A','n','d',' ','m','o','r','e',' ','b','y','t','e',' ','t','o','h','a','s','h',' 
','a','n','d',' ','s','e','e','.','.','.',0};
uint8_t i;

void setup() {

    mySerial.begin(9600);

    ME.writeEnable();
    for (i=0; i<64 ; i++)
      ME.write(i,datas[i]);
    ME.writeDisable();
}

void loop() {

    uint8_t problem=0;
    
    // read the value back
    for (i=0; i<64 ; i++)
      if (ME.read(i)!=datas[i])
        problem=0xFF;

    if (problem)
      mySerial.write('E');
    else
      mySerial.write('O');

    delay(500);

}
