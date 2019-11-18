/*
  MicrowireEEPROM Example Sketch
 Reads and writes a Microwire EEPROM.
 Written by Timo Schneider <timos@perlplexity.org> 
 
 This code is in the public domain.
*/


#include <MicrowireEEPROM.h>

// Microwire needs four wires (apart from VCC/GND) DO,DI,CS,CLK
// configure them here, note that DO and DI are the pins of the
// EEPROM, so DI is an output of the uC, while DO is an input
int CS=13; int CLK=12; int DI=7; int DO=2;

// EEPROMS have different sizes. Also the number of bits per page varies.
// We need to configure the page size in bits (PGS) and address bus width
// in bits (ADW). The speed at which the clock is run is configured in
// microseconds.
int PGS=16; int ADW=8; int SPD=200;

// initialize the library
MicrowireEEPROM ME(CS, CLK, DI, DO, PGS, ADW, SPD); 

void setup() {
  Serial.begin(9600);
}

void loop() {
  // write a value to each address
  for (int addr=0; addr < (1<<ADW); addr++) {
   
    int w = (42 + addr);
    // writeEnable must be called before the first write after initialization
    // or a call to writeDisable
    ME.writeEnable();
    ME.write(addr, w);
    // call writeDisable to prevent writing by accident
    ME.writeDisable();
    
    // read the value back
    int r = ME.read(addr);
    
    // give some debug output
    Serial.print("Address "); 
    Serial.print(addr, HEX);
    Serial.print(" written ");
    Serial.print(w, HEX);
    Serial.print(" read ");
    Serial.print(r, HEX);
    if (r == w) Serial.println(" ok");
    else Serial.println(" NOT OK");
    
    // EEPROMS wear out, so don't write too fast
    delay(1000);
  }
  
}

