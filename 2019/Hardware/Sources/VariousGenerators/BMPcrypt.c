#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

//uint8_t tab[16] = {0x12, 0x99, 0xC4, 0x6B, 0x5C, 0x27, 0x76, 0x4E, 0x64, 0xF2, 0x03, 0x21, 0x41, 0x17, 0x7F, 0x2E}; // Glasses.bmp
//uint8_t tab[16] = {0x43,0xAF,0x57,0x72,0xFB,0xB6,0xD0,0x28,0x94,0xF0,0x27,0x61,0x98,0xA9,0xBD,0xDB}; // LLAMA SAYS
uint8_t tab[16] = {0xF9,0x9F,0x6D,0xB2,0x2C,0x0C,0xB4,0x90,0x3A,0xCB,0x7E,0x65,0x9E,0x50,0xB2,0x65}; // Windoz



// based on a 16 bytes keys, return 8 differents keys
uint8_t getKeyByte(uint32_t offset)
{
	uint8_t byte;

	byte=offset%128;

	if (byte/16==0)
		return(tab[byte%16]);
	return( (tab[byte%16]<<(byte/16)) | (tab[byte%16]>>(8-byte/16)) );
}

int main() {

const char * filename  = "./Windoz.bmp";
const char * filename2 = "./WindozCrypt.bmp";

int returnCode;

FILE * stream = fopen( filename, "r" );

char buffer[1086];
int i;

    if ( stream == NULL ) {
        fprintf( stderr, "Cannot open file for reading\n" );
        exit( -1 );
    }


    if ( 1086 != fread( buffer, 1, 1086, stream ) ) {
        fprintf( stderr, "Cannot read 1086 bytes in file\n" );
    }

    returnCode = fclose( stream );
    if ( returnCode == EOF ) {
        fprintf( stderr, "Cannot close file\n" );
        exit( -1 );
    }

	// crypt the pixels only zone of the BMP, len 1024 for a 128x64x1
	// add enought noise to discourage cryptanalyse during a CTF :)
	for (i=0;i<1024;i++)
		buffer[i+0x3E]^=(getKeyByte(i)^i^i>>2);


    stream = fopen( filename2, "w" );
    if ( stream == NULL ) {
        fprintf( stderr, "Cannot open file for writing\n" );
        exit( -1 );
    }

    if ( 1086 != fwrite( buffer, 1, 1086, stream ) ) {
        fprintf( stderr, "Cannot write block in file\n" );
    }

    returnCode = fclose( stream );
    if ( returnCode == EOF ) {
        fprintf( stderr, "Cannot close file\n" );
        exit( -1 );
    }
}
