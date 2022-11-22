#ifndef _COMPRESS_H_
#define _COMPRESS_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "input.h"
#include "huffman.h"

/* =============== Macro definitions =============== */

#define PAGE_SIZE 4096

#define START_ADDR 0xcafebabe

#define BITS_SIZE(x) (sizeof(x) * 8)

/* ================ Type definitions =============== */

struct char_encoding {
    uint64_t value;
    uint8_t nb_bits;
};

struct compressed_buffer {
    uint64_t buffer_size;
    uint8_t *buffer;
    struct char_encoding **map;
};

/* ============== Function definitions ============= */

struct compressed_buffer *compress(struct source_file *source, struct huffman_tree_node *tree);

#endif /* _COMPRESS_H_ */

