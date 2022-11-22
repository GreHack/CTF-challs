#ifndef _INPUT_H_
#define _INPUT_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/* =============== Macro definitions =============== */

/* ================ Type definitions =============== */

struct source_file {
    int fd;
    size_t size;
    uint8_t *map;
    uint64_t *occurences;
};

/* ============== Function definitions ============= */

struct source_file *parse_input_file(char *filename);

#endif /* _INPUT_H_ */

