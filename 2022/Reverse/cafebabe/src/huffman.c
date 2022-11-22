#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "input.h"
#include "huffman.h"

struct huffman_tree_node *alloc_entry(uint8_t value, uint64_t nb_occurences, struct huffman_tree_node *next, struct huffman_tree_node *left, struct huffman_tree_node *right) {

    struct huffman_tree_node *ret = malloc(sizeof(struct huffman_tree_node));

    memset(ret, 0, sizeof(struct huffman_tree_node));
    ret->left = left;
    ret->right = right;
    ret->next = next;
    ret->value = value;
    ret->nb_occurences = nb_occurences;

#ifdef DEBUG
    printf("[*] Value %x - Number of occurences %lx\n", value, nb_occurences);
#endif

    return ret;
}

void insert_and_sort(struct huffman_tree_node **list, uint8_t value, uint64_t nb_occurences, struct huffman_tree_node *left, struct huffman_tree_node *right) {

    struct huffman_tree_node *last_entry = NULL;

    for (struct huffman_tree_node *entry = *list; entry != NULL; entry = entry->next) {
        if (nb_occurences <= entry->nb_occurences)
            break;
        last_entry = entry;
    }

    if (last_entry) {
        last_entry->next = alloc_entry(value, nb_occurences, last_entry->next, left, right);
    } else {
        *list = alloc_entry(value, nb_occurences, *list, left, right);
    }
}

struct huffman_tree_node *array_to_sorted_list(uint64_t *occurences) {

    struct huffman_tree_node *sorted_list = NULL;

    for (uint16_t i = 0; i <= UINT8_MAX; i++)
        if (occurences[i])
            insert_and_sort(&sorted_list, i, occurences[i], NULL, NULL);

    return sorted_list;
}

struct huffman_tree_node *get_tree(struct source_file *source) {

    struct huffman_tree_node *sorted_occurences_list = NULL;
    struct huffman_tree_node *left = NULL;
    struct huffman_tree_node *right = NULL;

    sorted_occurences_list = array_to_sorted_list(source->occurences);

    while (true) {
        left = sorted_occurences_list;
        right = sorted_occurences_list->next;
        sorted_occurences_list = right->next;
        insert_and_sort(&sorted_occurences_list, 0, left->nb_occurences + right->nb_occurences, left, right);

        if (!(sorted_occurences_list->next))
            break;
    }

    return sorted_occurences_list;
}
