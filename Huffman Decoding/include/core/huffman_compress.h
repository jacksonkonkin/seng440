#ifndef HUFFMAN_COMPRESS_H
#define HUFFMAN_COMPRESS_H

#include <stdio.h>
#include "encoder.h"
#include "decoder.h"
#include "file_format.h"

// High-level compression/decompression interface

typedef struct huffman_context {
    frequency_table_t* freq_table;
    code_table_t* code_table;
    encoder_node_t* tree_root;
    huffman_tree_t* decode_tree;
    bit_writer_t* writer;
} huffman_context_t;

// Context management
huffman_context_t* huffman_context_create(void);
void huffman_context_destroy(huffman_context_t* ctx);

// File compression
int huffman_compress_file(const char* input_path, const char* output_path);
int huffman_compress_data(const uint8_t* data, size_t data_size, 
                         uint8_t** compressed_data, size_t* compressed_size,
                         symbol_info_t** symbol_table, size_t* symbol_count);

// File decompression  
int huffman_decompress_file(const char* input_path, const char* output_path);
int huffman_decompress_data(const uint8_t* compressed_data, size_t compressed_size,
                           const symbol_info_t* symbol_table, size_t symbol_count,
                           uint8_t** output_data, size_t* output_size, size_t expected_size);

// Utility functions
void print_compression_stats(size_t original_size, size_t compressed_size);
int validate_huffman_file(const char* path);

#endif