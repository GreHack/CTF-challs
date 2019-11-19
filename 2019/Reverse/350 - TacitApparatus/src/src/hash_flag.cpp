// Generate the hashed flag.
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "model_hasher.h"

int main(int argc, char* argv[])
{
  if(argc != 2)
  {
    fprintf(stderr, "Usage: %s <flag_to_hash>\n", argv[0]);
    return 1;
  }

  if(strlen(argv[1]) > 64)
  {
    fprintf(stderr, "Input is too big.\n");
    return 1;
  }

  uint8_t buffer[64] {};
  strcpy((char*)buffer, argv[1]);

  hash(buffer);

  printf("const uint8_t HashedFlag[] = {");

  for(int i = 0; i < N; ++i)
    printf("0x%.2X, ", buffer[i]);

  printf("};\n");

  return 0;
}

