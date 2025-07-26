#include "decoder.h"
#include <stdlib.h>
#include <string.h>

huffman_decoder_t* huffman_decoder_create(huffman_tree_t* tree) {
    huffman_decoder_t* decoder = malloc(sizeof(huffman_decoder_t));
    if (!decoder) return NULL;
    
    decoder->tree = tree;
    decoder->output_capacity = 1024;
    decoder->output_buffer = malloc(decoder->output_capacity);
    decoder->output_size = 0;
    
    if (!decoder->output_buffer) {
        free(decoder);
        return NULL;
    }
    
    return decoder;
}

void huffman_decoder_destroy(huffman_decoder_t* decoder) {
    if (!decoder) return;
    
    if (decoder->output_buffer) {
        free(decoder->output_buffer);
    }
    free(decoder);
}

static int resize_output_buffer(huffman_decoder_t* decoder) {
    size_t new_capacity = decoder->output_capacity * 2;
    uint8_t* new_buffer = realloc(decoder->output_buffer, new_capacity);
    if (!new_buffer) return -1;
    
    decoder->output_buffer = new_buffer;
    decoder->output_capacity = new_capacity;
    return 0;
}

int huffman_decode_symbol(huffman_tree_t* tree, bit_stream_t* stream, uint8_t* symbol) {
    if (!tree || !tree->root || !stream || !symbol) return -1;
    
    huffman_node_t* current = tree->root;
    
    while (!current->is_leaf) {
        if (!bit_stream_has_data(stream)) {
            return -1;
        }
        
        bool bit = bit_stream_read_bit(stream);
        
        if (bit) {
            current = current->right;
        } else {
            current = current->left;
        }
        
        if (!current) {
            return -1;
        }
    }
    
    *symbol = current->symbol;
    return 0;
}

int huffman_decode(huffman_decoder_t* decoder, bit_stream_t* input, uint8_t** output, size_t* output_size) {
    if (!decoder || !input || !output || !output_size) return -1;
    
    decoder->output_size = 0;
    
    while (bit_stream_has_data(input)) {
        uint8_t symbol;
        int result = huffman_decode_symbol(decoder->tree, input, &symbol);
        
        if (result != 0) {
            break;
        }
        
        if (decoder->output_size >= decoder->output_capacity) {
            if (resize_output_buffer(decoder) != 0) {
                return -1;
            }
        }
        
        decoder->output_buffer[decoder->output_size++] = symbol;
    }
    
    *output = decoder->output_buffer;
    *output_size = decoder->output_size;
    
    return 0;
}