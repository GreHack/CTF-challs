#!/bin/bash

cc -std=gnu99 -Wall -Wextra -O2 -pedantic   -c -o gpio.o gpio.c
cc -std=gnu99 -Wall -Wextra -O2 -pedantic   -c -o lcd.o lcd.c
cc -std=gnu99 -Wall -Wextra -O2 -pedantic   -c -o lcd_lowlevel.o lcd_lowlevel.c
cc -std=gnu99 -Wall -Wextra -O2 -pedantic   -c -o lcd_cgram.o lcd_cgram.c
cc -std=gnu99 -Wall -Wextra -O1 -pedantic -c -o cheapOTP.o cheapOTP.c
cc cheapOTP.o gpio.o lcd.o lcd_lowlevel.o lcd_cgram.o -o cheapOTP