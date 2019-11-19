// Native version of the challenge,
// directly loadable and runnable by Angr.
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "model_hashed_flag.h"
#include "model_hasher.h"

int main(int argc, char* argv[])
{
  if(argc != 2)
  {
    fprintf(stderr, "no input\n");
    return 1;
  }

  uint8_t buffer[64] {};
  strcpy((char*)buffer, argv[1]);

  hash(buffer);

  if(memcmp(buffer, HashedFlag, N))
    return 2;

  printf("Yay\n");
  return 0;
}

