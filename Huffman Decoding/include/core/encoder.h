#ifndef ENCODER_H
#define ENCODER_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define MAX_SYMBOLS 256
#define MAX_CODE_LENGTH 32

typedef struct frequency_table {
    uint64_t frequencies[MAX_SYMBOLS];
    size_t unique_symbols;
} frequency_table_t;

typedef struct huffman_code {
    uint32_t code;        // The Huffman code
    uint8_t length;       // Code length in bits
    bool valid;           // Whether this symbol has a code
} huffman_code_t;

typedef struct code_table {
    huffman_code_t codes[MAX_SYMBOLS];
    uint8_t max_length;
} code_table_t;

typedef struct encoder_node {
    uint64_t frequency;
    uint8_t symbol;
    bool is_leaf;
    struct encoder_node* left;
    struct encoder_node* right;
} encoder_node_t;

typedef struct bit_writer {
    uint8_t* buffer;
    size_t buffer_size;
    size_t buffer_capacity;
    uint64_t bit_buffer;
    uint8_t bits_in_buffer;
} bit_writer_t;

// Frequency analysis
frequency_table_t* frequency_table_create(void);
void frequency_table_destroy(frequency_table_t* table);
int frequency_table_analyze(frequency_table_t* table, const uint8_t* data, size_t length);

// Huffman tree building
encoder_node_t* build_huffman_tree(const frequency_table_t* freq_table);
void destroy_encoder_tree(encoder_node_t* root);

// Code generation
code_table_t* generate_codes(encoder_node_t* root);
void code_table_destroy(code_table_t* table);

// Canonical Huffman
int make_canonical(code_table_t* table);

// Bit writing
bit_writer_t* bit_writer_create(void);
void bit_writer_destroy(bit_writer_t* writer);
int bit_writer_write_bits(bit_writer_t* writer, uint32_t bits, uint8_t count);
int bit_writer_write_code(bit_writer_t* writer, const huffman_code_t* code);
int bit_writer_flush(bit_writer_t* writer);
uint8_t* bit_writer_get_data(bit_writer_t* writer, size_t* size);

#endif