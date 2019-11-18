// index generator to hide the firmware for LVL8

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

int main() {

const char * filename  = "24C64final.bin";
const char * filename2 = "index.bin";


FILE * stream = fopen( filename, "r" );
int returnCode;

unsigned char buffer[8192];
short i,j,index[256];

    if ( stream == NULL ) {
        fprintf( stderr, "Cannot open file for reading\n" );
        exit( -1 );
    }


    if ( 8192 != fread( buffer, 1, 8192, stream ) ) {
        fprintf( stderr, "Cannot read 8192 bytes in file\n" );
    }

    returnCode = fclose( stream );
    if ( returnCode == EOF ) {
        fprintf( stderr, "Cannot close file\n" );
        exit( -1 );
    }

	
	// search each byte [00..FF] in the 24C64
	// from the end to the top
	// and keep each offset only
	for (i=0;i<256;i++)
	{
		for (j=8191;j>=0;j--)
		{
			if (buffer[j]==(unsigned char)i)
			{
				index[i]=j;
				break;
			}
		}
	}

	// min and max offset to check needed anthropie (11 bits)
	int min=8193,max=0;
	for (i=0;i<256;i++)
	{
		if (index[i]>max)
			max=index[i];
		if (index[i]<min)
			min=index[i];
		printf("byte %d at offset %d\n",i,index[i]);
	}
	printf("min:%d, max:%d\n",min,max);

    stream = fopen( filename2, "w" );
    if ( stream == NULL ) {
        fprintf( stderr, "Cannot open file for writing\n" );
        exit( -1 );
    }

    if ( 512 != fwrite( index, 1, 512, stream ) ) {
        fprintf( stderr, "Cannot write block in file\n" );
    }

    returnCode = fclose( stream );
    if ( returnCode == EOF ) {
        fprintf( stderr, "Cannot close file\n" );
        exit( -1 );
    }
}
