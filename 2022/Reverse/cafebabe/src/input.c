#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>

#include "log.h"
#include "input.h"

uint64_t *count_occurences(uint8_t *text, uint64_t text_size) {

    uint64_t *occurences = calloc(UINT8_MAX + 1, sizeof(uint64_t));

    if (occurences == NULL)
        do_error_exit("calloc");

    for (uint64_t i = 0; i < text_size; i++) 
        occurences[text[i]]++;

    return occurences;
}

struct source_file *parse_input_file(char *filename) {

    struct source_file *src = malloc(sizeof(struct source_file));

    if (src == NULL)
        do_error_exit("malloc");

    memset(src, 0, sizeof(struct source_file));

    src->fd = open(filename, O_RDONLY);

    if (src->fd < 0)
        do_error_exit("open");

    src->size = lseek(src->fd, 0, SEEK_END);
    if (src->size < 0)
        do_error_exit("lseek");

#ifdef DEBUG
    printf("[*] Input file size %d bytes \n", src->size);
#endif

    src->map = mmap(NULL, src->size, PROT_READ, MAP_PRIVATE, src->fd, 0);
    if (src->map == MAP_FAILED)
        do_error_exit("mmap");

    src->occurences = count_occurences(src->map, src->size);

    return src;
}

