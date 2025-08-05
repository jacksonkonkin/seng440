#include "huffman_tree.h"
#include <stdlib.h>
#include <string.h>

huffman_tree_t* huffman_tree_create() {
    huffman_tree_t* tree = malloc(sizeof(huffman_tree_t));
    if (!tree) return NULL;
    
    tree->root = NULL;
    tree->node_count = 0;
    return tree;
}

huffman_node_t* huffman_node_create(uint8_t symbol, bool is_leaf) {
    huffman_node_t* node = malloc(sizeof(huffman_node_t));
    if (!node) return NULL;
    
    node->symbol = symbol;
    node->is_leaf = is_leaf;
    node->left = NULL;
    node->right = NULL;
    return node;
}

static void huffman_node_destroy(huffman_node_t* node) {
    if (!node) return;
    
    huffman_node_destroy(node->left);
    huffman_node_destroy(node->right);
    free(node);
}

void huffman_tree_destroy(huffman_tree_t* tree) {
    if (!tree) return;
    
    huffman_node_destroy(tree->root);
    free(tree);
}

huffman_tree_t* huffman_tree_from_codes(uint8_t* symbols, uint8_t* code_lengths, size_t count) {
    huffman_tree_t* tree = huffman_tree_create();
    if (!tree) return NULL;
    
    tree->root = huffman_node_create(0, false);
    if (!tree->root) {
        huffman_tree_destroy(tree);
        return NULL;
    }
    
    uint32_t code = 0;
    
    for (size_t i = 0; i < count; i++) {
        if (code_lengths[i] == 0) continue;
        
        huffman_node_t* current = tree->root;
        
        for (int bit = code_lengths[i] - 1; bit >= 0; bit--) {
            uint32_t bit_val = (code >> bit) & 1;
            
            if (bit_val == 0) {
                if (!current->left) {
                    current->left = huffman_node_create(0, false);
                    if (!current->left) {
                        huffman_tree_destroy(tree);
                        return NULL;
                    }
                }
                current = current->left;
            } else {
                if (!current->right) {
                    current->right = huffman_node_create(0, false);
                    if (!current->right) {
                        huffman_tree_destroy(tree);
                        return NULL;
                    }
                }
                current = current->right;
            }
        }
        
        current->symbol = symbols[i];
        current->is_leaf = true;
        code++;
    }
    
    return tree;
}

huffman_tree_t* huffman_tree_from_code_table(uint8_t* symbols, uint32_t* codes, uint8_t* code_lengths, size_t count) {
    huffman_tree_t* tree = huffman_tree_create();
    if (!tree) return NULL;
    
    tree->root = huffman_node_create(0, false);
    if (!tree->root) {
        huffman_tree_destroy(tree);
        return NULL;
    }
    
    for (size_t i = 0; i < count; i++) {
        if (code_lengths[i] == 0) continue;
        
        huffman_node_t* current = tree->root;
        
        // Traverse the tree using the actual code bits
        for (int bit = code_lengths[i] - 1; bit >= 0; bit--) {
            uint32_t bit_val = (codes[i] >> bit) & 1;
            
            if (bit_val == 0) {
                if (!current->left) {
                    current->left = huffman_node_create(0, false);
                    if (!current->left) {
                        huffman_tree_destroy(tree);
                        return NULL;
                    }
                }
                current = current->left;
            } else {
                if (!current->right) {
                    current->right = huffman_node_create(0, false);
                    if (!current->right) {
                        huffman_tree_destroy(tree);
                        return NULL;
                    }
                }
                current = current->right;
            }
        }
        
        current->symbol = symbols[i];
        current->is_leaf = true;
    }
    
    return tree;
}