#ifndef _OUTPUT_H_
#define _OUTPUT_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "compress.h"

/* =============== Macro definitions =============== */

#define MAGIC "\xca\xfe\xba\xbe"

#define ALIGN(value, n) (((value) + ((n) - 1)) & ~((n) - 1))
#define MASK(nb_bits) ((1 << (nb_bits)) - 1)

/* ================ Type definitions =============== */

struct encoding_table_entry {
    uint8_t original_value;
    uint64_t new_value;
    uint64_t mask;
};

/* ============== Function definitions ============= */

void write_compressed_file(const char *filename, struct compressed_buffer *compressed_data);

#endif /* _OUTPUT_H_ */

