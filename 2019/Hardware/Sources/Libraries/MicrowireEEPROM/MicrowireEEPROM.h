/*
  MicrowireEEPROM
 Reads and writes a Microwire EEPROM.
 Written by Timo Schneider <timos@perlplexity.org> 
 
 This example code is in the public domain.
 */

#ifndef MICROWIREEEPROM_H
#define MICROWIREEEPROM_H

#include "Arduino.h"

class MicrowireEEPROM {
  private:
  // Microwire needs four wires (apart from VCC/GND) DO,DI,CS,CLK
  // configure them here, note that DO and DI are the pins of the
  // EEPROM, so DI is an output of the uC, while DO is an input
  int CS;
  int CLK;
  int DI;
  int DO;
  // an EEPROM can have a varying pagesize, usually 8 or 16 bit per page
  int PAGESIZE;
  // an EEPROM can have a varying address width (depending on its storage size)
  int ADDRWIDTH;
  // (half of) the clock period in us
  int HALF_CLOCK_PERIOD;

  long transmit(int data, int bits);
  void send_opcode(char op);

  public:
  MicrowireEEPROM(int cs_pin, int clk_pin, int di_pin, int do_pin, int pg_size,
                  int addr_width, int clock_period);
  int read(int addr);
  void writeEnable(void);
  void writeDisable(void);
  void write(int addr, int data);
};

#endif
