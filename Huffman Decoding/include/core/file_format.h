#ifndef FILE_FORMAT_H
#define FILE_FORMAT_H

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

#define HUFFMAN_MAGIC 0x48554646  // "HUFF"
#define HUFFMAN_VERSION 1

typedef struct huffman_header {
    uint32_t magic;           // Magic number: "HUFF"
    uint16_t version;         // Format version
    uint16_t flags;           // Reserved flags
    uint64_t original_size;   // Original uncompressed size
    uint64_t compressed_size; // Compressed data size (excluding header)
    uint16_t symbol_count;    // Number of symbols with non-zero frequency
    uint16_t max_code_length; // Maximum code length in bits
    uint32_t checksum;        // CRC32 of original data
} __attribute__((packed)) huffman_header_t;

typedef struct symbol_info {
    uint8_t symbol;      // The symbol value
    uint8_t code_length; // Length of Huffman code in bits
    uint32_t code;       // The actual Huffman code
} __attribute__((packed)) symbol_info_t;

// File format:
// [huffman_header_t]
// [symbol_info_t array] - sorted by symbol value
// [compressed bit stream]

int huffman_write_header(FILE* file, const huffman_header_t* header);
int huffman_read_header(FILE* file, huffman_header_t* header);
int huffman_write_symbol_table(FILE* file, const symbol_info_t* symbols, size_t count);
int huffman_read_symbol_table(FILE* file, symbol_info_t* symbols, size_t count);
uint32_t calculate_crc32(const uint8_t* data, size_t length);

#endif