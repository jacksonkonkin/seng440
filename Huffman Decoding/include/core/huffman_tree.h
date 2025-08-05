#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct huffman_node {
    uint8_t symbol;
    bool is_leaf;
    struct huffman_node* left;
    struct huffman_node* right;
} huffman_node_t;

typedef struct huffman_tree {
    huffman_node_t* root;
    size_t node_count;
} huffman_tree_t;

huffman_tree_t* huffman_tree_create();
void huffman_tree_destroy(huffman_tree_t* tree);
huffman_node_t* huffman_node_create(uint8_t symbol, bool is_leaf);
huffman_tree_t* huffman_tree_from_codes(uint8_t* symbols, uint8_t* code_lengths, size_t count);
huffman_tree_t* huffman_tree_from_code_table(uint8_t* symbols, uint32_t* codes, uint8_t* code_lengths, size_t count);

#endif