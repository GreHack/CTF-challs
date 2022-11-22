#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#include "log.h"
#include "output.h"
#include "huffman.h"
#include "compress.h"

void write_compressed_file(const char *filename, struct compressed_buffer *compressed_data) {

    const uint64_t buffer_entry_offset = (sizeof(MAGIC) - 1) + (2 * sizeof(uint64_t));
    uint64_t buffer_size = ALIGN((compressed_data->buffer_size >> 3) + 1, 8);
    uint64_t huffman_tree_offset = buffer_entry_offset + sizeof(uint64_t) + buffer_size;
    struct encoding_table_entry tmp_buffer = {};
    int fd = open(filename, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);

    if (fd < 0)
        do_error_exit("open");

    // Write magic
    if (write(fd, MAGIC, sizeof(MAGIC) - 1) < 0)
        do_error_exit("write");

    // Compressed buffer offset
    if (write(fd, &buffer_entry_offset, sizeof(uint64_t)) < 0)
        do_error_exit("write");

    // huffman tree offset
    if (write(fd, &huffman_tree_offset, sizeof(uint64_t)) < 0)
        do_error_exit("write");

    // Buffer size
    if (write(fd, &(compressed_data->buffer_size), sizeof(uint64_t)) < 0)
        do_error_exit("write");

    // Buffer
    if(write(fd, compressed_data->buffer, buffer_size) < 0)
        do_error_exit("write");

    // Huffman tree
    for (uint16_t i = 0; i <= UINT8_MAX; i++) {
        if (compressed_data->map[i]) {
            tmp_buffer.original_value = (uint8_t)i;
            tmp_buffer.new_value = compressed_data->map[i]->value;
            tmp_buffer.mask = MASK(compressed_data->map[i]->nb_bits);

#ifdef DEBUG
            printf("[*] Write entry %x with value %lx and mask %lx\n", i, tmp_buffer.new_value, tmp_buffer.mask);
#endif

            if(write(fd, &tmp_buffer, sizeof(struct encoding_table_entry)) < 0)
                do_error_exit("write");
        }
    }

    close(fd);

}

