#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>

#include "sha256.c"


const char cst1[] = "\xa0\x26\x2d\xfd\x90\x77\xdd\x32\xa3\x05\xd9\x69\xa9\x46\x9b\xf1\x65\xc1\x26\xe8\xe6\xad\x83\x41\x60\x22\x97\x8b\x0b\xce\x9a\x39";
const char cst2[] = "\xEB\x59\x36\x1F\xC3\x36\xE2\x4B\xEA\x27";

FILE *file;
char OTPserial[9]="GreHack!";
char hashBuf[32];
char cur[32];
char buffer[11];
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

int main()
{

	int i,j;

	buffer[10]=0;

	readOTP();
	hash2(OTPserial,8,hashBuf);
	memcpy(cur,hashBuf,32);

	printf("\n\n*** GreHack2017's OTP values ***\n\nInit : ");
	for (i = 0 ; i<32 ; i++)
		printf("%02X", cur[i]);

	printf("\n");

	for (j = 0 ; j<40 ; j++)
	{
		printf("Round %03d ",seq);
		for (i = 0 ; i<32 ; i++)
			cur[i]^=cst1[i];
		// todo hour here
		hash2(cur,32,hashBuf);
		memcpy(cur,hashBuf,32);

		for (i = 0 ; i<32 ; i++)
			printf("%02X", cur[i]);
		
		printf(" ");

	        for (i = 0 ; i<10 ; i++)
		{
			buffer[i]= cur[i]^cst2[i];
			printf("%02X", buffer[i]);
		}

		printf(" ");

	        for (i = 0 ; i<10 ; i++)
		{
			buffer[i]= buffer[i]%26 + 'a';
		}
		printf("%s", buffer);
		//sleep(1);

		printf("\n");

		seq++;
	}

	printf("cst2 = ");
	char goodRandom[10] = "\x28\x1B\xFF\x58\x53\x74\x6F\x7F\x23\x1F";

	char flag[11] = "nottoolate";

	for (i = 0 ; i<10 ; i++)
	{
		if (i & 1)
			buffer[i]= (((flag[i]-'a')%26+26*2)^goodRandom[i]);
		else
			if (i & 4)
				buffer[i]= (((flag[i]-'a')%26+26*5)^goodRandom[i]);
			else
				buffer[i]= (((flag[i]-'a')%26+26*7)^goodRandom[i]);
		printf("\\x%02X", buffer[i]);
	}

	printf("\n");
}
