# Microwire EEPROM Library for Arduino

This library enables you to read and write EEPROM chips which use the Microwire
protocol. Examples are chips such as the ST93C66 or 93LC46. See the datasheets
of these chips for a description of the protocol.

## Why not just use SPI?

For some Microwire
EEPROMS you could use the Arduinos SPI interface, however, this does not work
in all cases --- some chips count the number of clock pulses between the start
bit and the falling edge of the clock signal. SPI works on multiples of eight
bits, so if the address width does not happen to be five (quite small) or
thirteen (quite large) bits, it will not work.

Therefore this library bitbangs the Microwire protocol. This also means you
have complete flexibility over which Arduino pins you want to use.

## Usage

The Microwire protocol uses four signals:
* chip select
* clock
* data in
* data out

When initializing this library, you need to assign pin numbers to these
signals. Furthermore, EEPROMs come in different variants, i.e., sizes and
how many bits are accessed per address (usually 8 or 16). You also need to
set the width of the address bus, the pagesize (bits per address) and speed
(clock period in microseconds).

    int CS=13; int CLK=12; int DI=7; int DO=2;
    int PGS=16; int ADW=8; int SPD=200;
    MicrowireEEPROM ME(CS, CLK, DI, DO, PGS, ADW, SPD); 

Afterwards you can use the functions read/write/writeEnable/writeDisable as
shown in the code below:

```
// writeEnable must be called before the first write 
ME.writeEnable();

// write a value to each address and read it back
for (int addr=0; addr < (1<<ADW); addr++) {
  int w = (42 + addr);
  ME.write(addr, w);
  int r = ME.read(addr);
}

// prevent writing by accident
ME.writeDisable();
```

Also see the example sketch for a complete code.

