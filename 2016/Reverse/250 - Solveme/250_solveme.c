#include <stdio.h>
#include <unistd.h>
#include <string.h>

/*
 * gcc -std=c99 -m32 250_solveme.c -o 250_solveme.bin
 * strip 250_solveme.bin
 */

int hash_code(char buf[]) {
  int n = strlen(buf);
  long sum = 23;
  for(int i=0; i<n; ++i) {
    sum = sum*7 + (buf[i] << i);
  }
  return sum >> 4;
}

int verify_flag(char buf[]){
  int n = strlen(buf);
  int sumA = 0;
  int sumB = 0;
  int xorAB = 0;
  int xorB = 0;
  int xorA = 0;
  int xorH = 0;
  int hash = 0;
  
  for(int i=0; i<n; ++i) {
    xorB = (i%2 == 0)?xorB:xorB ^ buf[i];
    xorA = (i%2 == 1)?xorA:xorA ^ buf[i];
    sumA += (i%2 == 0)?buf[i]:0;
    xorAB = (i%2 == 1)?xorAB ^ buf[i]:xorAB;
  }

  for(int i=0; i<(n/2); ++i) {
    xorH = xorH^buf[i];
  }
  
  for(int i=0; i<n; ++i) {
    sumB += (i%2 == 0)?0:buf[i];
    xorAB = (i%2 == 0)?xorAB ^ buf[i]:xorAB;
  }

  hash = hash_code(buf);

  if ( (sumA % 10) == 8
       && (sumB % 10) == (sumA %10)
       && n == 13
       && xorAB == 90
       && xorH == 21
       && xorB == 56
       && (sumA+sumB)/10 == 116
       && ((2*sumA+sumB) / 10) == 183
       && buf[1] == '3' && buf[5] == '5'
       && buf[8] == '4' && buf[10] == '7'
       && hash == -45192636) {
    return 1;
  }
  return 0;
}

int main (int argc, char* argv[]) {
  char buffer[32] = {0};
  int n;
  printf("Please entre the flag : ");
  fflush(stdout);
  fgets(buffer,32,stdin);
  for(int i=0; i<32; ++i) {
    buffer[i] += (buffer[i] == '\n')?-'\n':0;
  }
  if ( verify_flag(buffer) ) {
    printf("Well done, you can validate with :\nGH16{%s}\n",buffer);
  } else {
    printf("You entered %s but this is not the flag.\n",buffer);
  }
  
  return 0;
}
