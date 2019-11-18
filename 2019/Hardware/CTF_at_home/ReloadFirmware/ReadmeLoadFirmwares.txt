In this folder you’ll find some scripts to load self-test firmwares and the final version firmwares.

Self-test firmware behavior: 2 first lines gives status of all the hardware parts, 2 last lines print the second level flag (test access to the 24C64). All the LED’s glasses must blink. If nothing appears on OLED screen, this 
self-test firmware output debug on serial port too (via the USB port, speed: 9600 bauds).

If all is OK, you can load again finals firmware.

Those scripts run under windows and need to have Arduino’s official development environment installed. You also need to adjust some path and serial port, at the beginning of each .bat file.
For Linux, avrdude works the same way.
