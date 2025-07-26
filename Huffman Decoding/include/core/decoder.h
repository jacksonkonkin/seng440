#ifndef DECODER_H
#define DECODER_H

#include "huffman_tree.h"
#include "bit_stream.h"
#include <stdint.h>
#include <stddef.h>

typedef struct huffman_decoder {
    huffman_tree_t* tree;
    uint8_t* output_buffer;
    size_t output_size;
    size_t output_capacity;
} huffman_decoder_t;

huffman_decoder_t* huffman_decoder_create(huffman_tree_t* tree);
void huffman_decoder_destroy(huffman_decoder_t* decoder);
int huffman_decode(huffman_decoder_t* decoder, bit_stream_t* input, uint8_t** output, size_t* output_size);
int huffman_decode_symbol(huffman_tree_t* tree, bit_stream_t* stream, uint8_t* symbol);

#endif