#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/mman.h>

#include "log.h"
#include "input.h"
#include "huffman.h"
#include "compress.h"

struct char_encoding *alloc_encoding(uint64_t value, uint8_t nb_bits) {

    struct char_encoding *ret = malloc(sizeof(struct char_encoding *));
    if (!ret)
        do_error_exit("malloc");

    ret->value = value;
    ret->nb_bits = nb_bits;

    return ret;
}

struct compressed_buffer *allocate_compressed_buffer(uint64_t buffer_size, uint8_t *buffer, struct char_encoding **map) {

    struct compressed_buffer *ret = malloc(sizeof(struct compressed_buffer));
    if (!ret)
        do_error_exit("malloc");

    ret->buffer_size = buffer_size;
    ret->buffer = buffer;
    ret->map = map;

    return ret;
}

void iter_on_subtree(struct huffman_tree_node *tree, struct char_encoding **map, uint64_t value, uint8_t nb_bits) {

    // Base case
    if (!(tree->left) && !(tree->right)) {
        map[tree->value] = alloc_encoding(value, nb_bits);
    } else {
        iter_on_subtree(tree->left, map, value << 1, nb_bits + 1);
        iter_on_subtree(tree->right, map, (value << 1) + 1, nb_bits + 1);
    }
}

struct char_encoding **get_map(struct huffman_tree_node *tree) {

    struct char_encoding **ret = calloc(UINT8_MAX + 1, sizeof(struct char_encoding));

    iter_on_subtree(tree, ret, 0, 0);

    return ret;
}

struct compressed_buffer *function(struct char_encoding **map, struct source_file *source) {

    uint64_t nb_written_bits = 0;
    int8_t nb_remaining_bits = 0;
    uint64_t array_pos = 0;
    struct char_encoding *current_char = NULL;
    uint64_t *dest_buffer = (uint64_t *)mmap((void *)START_ADDR, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (dest_buffer == MAP_FAILED)
        do_error_exit("mmap");

    memset(dest_buffer, 0, PAGE_SIZE);

    for (uint64_t i = 0; i < source->size; i++) {
        current_char = map[source->map[i]];
        nb_remaining_bits = BITS_SIZE(dest_buffer[0]) - (nb_written_bits % BITS_SIZE(dest_buffer[0]));

        if (nb_remaining_bits < current_char->nb_bits) {
            dest_buffer[array_pos++] |= current_char->value >> (current_char->nb_bits - nb_remaining_bits);
            if ((array_pos * sizeof(dest_buffer[0])) % PAGE_SIZE == 0)
                if (mmap((void *)dest_buffer + (array_pos * sizeof(dest_buffer[0])), PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)
                        == MAP_FAILED)
                    do_error_exit("mmap");
        }

        dest_buffer[array_pos] |= current_char->value << (nb_remaining_bits - current_char->nb_bits);

        nb_written_bits += current_char->nb_bits;
        if (!(nb_written_bits % BITS_SIZE(dest_buffer[0])))
            array_pos++;
    }

    return allocate_compressed_buffer(nb_written_bits, (uint8_t *)dest_buffer, map);
}

struct compressed_buffer *compress(struct source_file *source, struct huffman_tree_node *tree) {

    struct char_encoding **map = get_map(tree);

    return function(map, source);
}
