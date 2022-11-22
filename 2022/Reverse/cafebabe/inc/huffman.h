#ifndef _HUFFMAN_H_
#define _HUFFMAN_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "input.h"

/* =============== Macro definitions =============== */

/* ================ Type definitions =============== */

struct huffman_tree_node {
    struct huffman_tree_node *next;
    struct huffman_tree_node *left;
    struct huffman_tree_node *right;
    uint64_t nb_occurences;
    uint8_t value;
};

struct list_entry {
    struct list_entry *next;
    struct list_entry *prev;
};

struct occurences_value_pair {
    struct list_entry list;
    uint8_t value;
    uint64_t nb_occurences;
};

/* ============== Function definitions ============= */

struct huffman_tree_node *get_tree(struct source_file *source);

#endif /* _HUFFMAN_H_ */

