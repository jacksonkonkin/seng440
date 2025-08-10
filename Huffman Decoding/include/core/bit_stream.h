#ifndef BIT_STREAM_H
#define BIT_STREAM_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

typedef struct bit_stream {
    uint8_t* data;
    size_t data_size;
    size_t byte_pos;
    uint8_t bit_pos;
    uint64_t bit_buffer;
    uint8_t bits_in_buffer;
} bit_stream_t;

bit_stream_t* bit_stream_create(uint8_t* data, size_t size);
void bit_stream_destroy(bit_stream_t* stream);
bool bit_stream_read_bit(bit_stream_t* stream);
uint32_t bit_stream_read_bits(bit_stream_t* stream, uint8_t num_bits);
bool bit_stream_has_data(bit_stream_t* stream);
void bit_stream_fill_buffer(bit_stream_t* stream);

// ITERATION 4: Lookup table support functions
uint32_t bit_stream_peek_bits(bit_stream_t* stream, uint8_t num_bits);
void bit_stream_skip_bits(bit_stream_t* stream, uint8_t num_bits);
int bit_stream_available_bits(bit_stream_t* stream);

// ITERATION 3: NEON SIMD batch processing functions
#ifdef __aarch64__
int bit_stream_read_bits_batch(bit_stream_t* stream, uint8_t* bit_counts, uint32_t* results, int batch_size);
void bit_stream_peek_batch_neon(bit_stream_t* stream, uint8_t* bit_counts, uint32_t* results, int batch_size);
void bit_stream_fill_buffer_neon(bit_stream_t* stream);
#endif

#endif