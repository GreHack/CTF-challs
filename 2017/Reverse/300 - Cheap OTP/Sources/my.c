#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>

#include "button.h"
#include "gpio.h"
#include "lcd.h"
#include "lcd_cgram.h"

#include "sha256.c"

#define waitRound 60*15	// 15 min
//#define waitRound 10

const char cst1[] = "\xa0\x26\x2d\xfd\x90\x77\xdd\x32\xa3\x05\xd9\x69\xa9\x46\x9b\xf1\x65\xc1\x26\xe8\xe6\xad\x83\x41\x60\x22\x97\x8b\x0b\xce\x9a\x39";
//const char cst2[] = "\x25\x15\xEC\x4B\x5D\x7A\x64\x7F\x30\x1B";
const char cst2[] = "\xEB\x59\x36\x1F\xC3\x36\xE2\x4B\xEA\x27";

FILE *file;
char OTPserial[9]="GreHack!";	// need to be overwritre with .ini file
char hashBuf[32];
char cur[32];
int seq;

void readOTP()
{
	file = fopen("OTPserial.ini", "r");
	fread(OTPserial, 1, 8, file);
	if (ferror(file))
		exit(1);
	fclose(file);
	OTPserial[8]=0;
}

void display()
{
	int i;
	char buffer[11];

	LCD_clear();
	LCD_home();

	LCD_putchar('O');
	LCD_putchar('T');
	LCD_putchar('P');
	LCD_putchar(' ');
	for (i = 0 ; i<8 ; i++)
		LCD_putchar(OTPserial[i]);
	LCD_putchar(' ');
	sprintf(buffer,"%03d",seq);
	LCD_putchar(buffer[0]);
	LCD_putchar(buffer[1]);
	LCD_putchar(buffer[2]);
	LCD_putchar('\n');
	LCD_putchar('G');
	LCD_putchar('H');
	LCD_putchar('1');
	LCD_putchar('7');
	LCD_putchar('{');
	for (i = 0 ; i<10 ; i++)
	{
		buffer[i]= (cur[i]^cst2[i])%26 + 'a';
		LCD_putchar(buffer[i]);
	}
	LCD_putchar('}');
}

int main()
{

	int i,k;

	GPIO_open();
	LCD_init(0);

	readOTP();
	hash2(OTPserial,8,hashBuf);
	memcpy(cur,hashBuf,32);

	while(1)
	{
		for (i = 0 ; i<32 ; i++)
			cur[i]^=cst1[i];
		hash2(cur,32,hashBuf);
		memcpy(cur,hashBuf,32);

		display();

		seq++;

		//wait + flash;
	for (k=0; k<waitRound ; k++)
	{
		LCD_colour(Green);
		usleep(900*1000);
		LCD_colour(Red);
		usleep(100*1000);
		
	}

	}
}

