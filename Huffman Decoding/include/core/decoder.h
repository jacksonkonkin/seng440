#ifndef DECODER_H
#define DECODER_H

#include "huffman_tree.h"
#include "bit_stream.h"
#include <stdint.h>
#include <stddef.h>

// ITERATION 3: NEON SIMD vectorized lookup tables
#ifdef __aarch64__
#include <arm_neon.h>
#endif

// Cache-optimized lookup table entry (64-byte aligned)
typedef struct __attribute__((packed, aligned(64))) lookup_entry {
    uint8_t symbol;           // Decoded symbol
    uint8_t code_length;      // Length of code in bits
    uint16_t padding;         // Padding for alignment
} lookup_entry_t;

// ITERATION 3: Vectorized lookup table for fast symbol decoding
typedef struct __attribute__((aligned(64))) vectorized_lookup_table {
    lookup_entry_t* direct_table;    // 12-bit direct lookup (4096 entries)
    lookup_entry_t* overflow_table;  // Overflow for longer codes
    size_t direct_size;              // Size of direct table
    size_t overflow_size;            // Size of overflow table
    uint8_t max_code_length;         // Maximum code length in bits
    uint8_t direct_bits;             // Bits used for direct lookup (12)
} vectorized_lookup_table_t;

typedef struct huffman_decoder {
    huffman_tree_t* tree;
    uint8_t* output_buffer;
    size_t output_size;
    size_t output_capacity;
    // ITERATION 3: NEON SIMD vectorized lookup table for fast decoding
    vectorized_lookup_table_t* lookup_table;
} huffman_decoder_t;

huffman_decoder_t* huffman_decoder_create(huffman_tree_t* tree);
void huffman_decoder_destroy(huffman_decoder_t* decoder);
int huffman_decode(huffman_decoder_t* decoder, bit_stream_t* input, uint8_t** output, size_t* output_size);
int huffman_decode_symbol(huffman_tree_t* tree, bit_stream_t* stream, uint8_t* symbol);

// ITERATION 3: NEON SIMD vectorized lookup table functions
vectorized_lookup_table_t* create_vectorized_lookup_table(huffman_tree_t* tree);
void destroy_vectorized_lookup_table(vectorized_lookup_table_t* table);
int vectorized_decode_symbol(vectorized_lookup_table_t* table, bit_stream_t* stream, uint8_t* symbol);

#ifdef __aarch64__
// NEON SIMD batch symbol decoding
int neon_decode_symbols_batch(vectorized_lookup_table_t* table, bit_stream_t* stream, uint8_t* symbols, int max_symbols);
#endif

#endif