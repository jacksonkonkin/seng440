#include "bit_stream.h"
#include <stdlib.h>
#include <string.h>

bit_stream_t* bit_stream_create(uint8_t* data, size_t size) {
    bit_stream_t* stream = malloc(sizeof(bit_stream_t));
    if (!stream) return NULL;
    
    stream->data = data;
    stream->data_size = size;
    stream->byte_pos = 0;
    stream->bit_pos = 0;
    stream->bit_buffer = 0;
    stream->bits_in_buffer = 0;
    
    return stream;
}

void bit_stream_destroy(bit_stream_t* stream) {
    if (stream) {
        free(stream);
    }
}

static void bit_stream_fill_buffer(bit_stream_t* stream) {
    while (stream->bits_in_buffer < 32 && stream->byte_pos < stream->data_size) {
        stream->bit_buffer |= ((uint64_t)stream->data[stream->byte_pos]) << (56 - stream->bits_in_buffer);
        stream->bits_in_buffer += 8;
        stream->byte_pos++;
    }
}

bool bit_stream_read_bit(bit_stream_t* stream) {
    if (stream->bits_in_buffer == 0) {
        bit_stream_fill_buffer(stream);
        if (stream->bits_in_buffer == 0) {
            return false;
        }
    }
    
    bool bit = (stream->bit_buffer & 0x8000000000000000ULL) != 0;
    stream->bit_buffer <<= 1;
    stream->bits_in_buffer--;
    
    return bit;
}

uint32_t bit_stream_read_bits(bit_stream_t* stream, uint8_t num_bits) {
    if (num_bits == 0 || num_bits > 32) return 0;
    
    uint32_t result = 0;
    
    for (uint8_t i = 0; i < num_bits; i++) {
        result <<= 1;
        if (bit_stream_read_bit(stream)) {
            result |= 1;
        }
    }
    
    return result;
}

bool bit_stream_has_data(bit_stream_t* stream) {
    return stream->bits_in_buffer > 0 || stream->byte_pos < stream->data_size;
}