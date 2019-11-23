#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

//final flag :

void decypher(char *string);

int main()
{
    char myflag[16] = "=>'/q'cJhWFf[Zs";
    char input[30] = "What is your favorite spider?";
    unsigned char thanks[1024] = "thanks a lot for this information\n";
    unsigned char passwd[128];
    
    decypher(myflag);    
    puts(input);
    fflush(stdout);
    fgets(passwd, 1152, stdin);
    printf(thanks);
    //printf(myflag);
    //printf("%-10s", input);

    return 0;
}

void decypher(char *string)
{
	int len = strlen(string);
	for(int i = 0; i < len; i++)
	{
		string[i] += 10;
	}
}
