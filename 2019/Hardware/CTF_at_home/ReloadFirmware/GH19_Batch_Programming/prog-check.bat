:: Batch to program and test GreHack's 2019 CTF boards
@echo off
echo.
echo.
echo.

set comPort=COM9
set runLine="C:\Program Files (x86)\Arduino\hardware\tools\avr\bin\avrdude.exe" -C"C:\Program Files (x86)\Arduino\hardware\tools\avr\etc\avrdude.conf"
set files=D:\GH19_Batch_Programming\

:playAgain

:: ask to plug the PCB
echo.
echo.
echo ***********************************************************
echo * Please connect the PCB to an USB port and press any key *
echo ***********************************************************
echo.
echo.
pause

:: Programing Arduino ISP firmware
echo.
echo.
echo ***********************************
echo * Programing Arduino ISP firmware *
echo ***********************************
echo.
echo.
%runline% -patmega328p -carduino -P%comPort% -b115200 -D -Uflash:w:%files%ArduinoISP.ino.hex:i
if not errorlevel 1 (
	echo.
) else (
	echo **** ERROR ****   Could not program Arduino ISP
	goto :playAgain
)

:: Programing ATtiny fuse bits
echo.
echo.
echo *******************************
echo * Programing ATtiny fuse bits *
echo *******************************
echo.
echo.
%runline% -pattiny85 -cstk500v1 -P%comPort% -b19200 -e -Uefuse:w:0xff:m -Uhfuse:w:0xdf:m -Ulfuse:w:0xe2:m 
if not errorlevel 1 (
	echo.
) else (
	echo **** ERROR ****   Could not program ATtiny fuse bits
	goto :playAgain
)


:: Programing ATtiny firmware {program + check 93c46}
echo.
echo.
echo ******************************************************
echo * Programing ATtiny firmware {program + check 93c46} *
echo ******************************************************
echo.
echo.
%runline% -pattiny85 -cstk500v1 -P%comPort% -b19200 -Uflash:w:%files%93xx-loader.ino.hex:i 
if not errorlevel 1 (
	echo.
) else (
	echo **** ERROR ****   Could not program ATtiny firmware {program + check 93c46}
	goto :playAgain
)


:: Programing Arduino test firmware
echo.
echo.
echo ************************************
echo * Programing Arduino test firmware *
echo ************************************
echo.
echo.
%runline% -patmega328p -carduino -P%comPort% -b115200 -D -Uflash:w:%files%check-hardware.ino.hex:i 
if not errorlevel 1 (
	echo.
) else (
	echo **** ERROR ****   Could not program Arduino test firmware
	goto :playAgain
)


goto :playAgain
