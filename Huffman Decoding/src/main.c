#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffman_tree.h"
#include "bit_stream.h" 
#include "decoder.h"

int main(int argc, const char* argv[]) {
    printf("Simple Huffman Decoder Test\n");
    printf("===========================\n\n");
    
    printf("Creating simple test case...\n");
    
    uint8_t symbols[] = {'A', 'B', 'C', 'D'};
    uint8_t code_lengths[] = {2, 2, 2, 2};
    size_t symbol_count = 4;
    
    printf("Building Huffman tree from codes:\n");
    printf("  A: 2 bits, B: 2 bits, C: 2 bits, D: 2 bits\n");
    
    huffman_tree_t* tree = huffman_tree_from_codes(symbols, code_lengths, symbol_count);
    if (!tree) {
        printf("Failed to create Huffman tree\n");
        return 1;
    }
    
    printf("Tree created successfully\n");
    
    uint8_t test_data[] = {0b00011011, 0b10000000};
    size_t test_data_size = 2;
    
    printf("Test data: 0x%02X 0x%02X (bits: 00 01 10 11 10)\n", 
           test_data[0], test_data[1]);
    printf("Expected decode: A B C D C\n");
    
    bit_stream_t* stream = bit_stream_create(test_data, test_data_size);
    if (!stream) {
        printf("Failed to create bit stream\n");
        huffman_tree_destroy(tree);
        return 1;
    }
    
    huffman_decoder_t* decoder = huffman_decoder_create(tree);
    if (!decoder) {
        printf("Failed to create decoder\n");
        bit_stream_destroy(stream);
        huffman_tree_destroy(tree);
        return 1;
    }
    
    uint8_t* output;
    size_t output_size;
    
    printf("Decoding...\n");
    int result = huffman_decode(decoder, stream, &output, &output_size);
    
    if (result == 0) {
        printf("Decode successful! Output (%zu symbols): ", output_size);
        for (size_t i = 0; i < output_size; i++) {
            printf("%c ", output[i]);
        }
        printf("\n");
    } else {
        printf("Decode failed with error %d\n", result);
    }
    
    huffman_decoder_destroy(decoder);
    bit_stream_destroy(stream);
    huffman_tree_destroy(tree);
    
    printf("\nBasic functionality test completed.\n");
    printf("Ready for optimization work!\n");
    
    return 0;
}