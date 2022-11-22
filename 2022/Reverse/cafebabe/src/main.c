#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/mman.h>

#include "log.h"
#include "input.h"
#include "output.h"
#include "huffman.h"
#include "compress.h"

void print_usage(char *argv0) {
    printf("Usage: %s <file-to-compress> <output-file>\n", argv0);
}

int main(int argc, char **argv) {

    struct source_file *source = NULL;
    struct huffman_tree_node *tree = NULL;
    struct compressed_buffer *compressed_data = NULL;

    if (argc != 3) {
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    // Read the source file
    source = parse_input_file(argv[1]);

    // Build Huffman tree
    tree = get_tree(source);

    // Compress data
    compressed_data = compress(source, tree);

    // Write the compress file
    write_compressed_file(argv[2], compressed_data);
}

