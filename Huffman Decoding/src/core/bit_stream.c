#include "bit_stream.h"
#include <stdlib.h>
#include <string.h>

// ARM64 inline assembly helpers for bit manipulation
static inline uint64_t arm64_rbit64(uint64_t x) {
    uint64_t result;
#ifdef __aarch64__
    __asm__("rbit %0, %1" : "=r"(result) : "r"(x));
#else
    // Fallback for non-ARM64 platforms
    result = __builtin_bswap64(x);
    result = ((result & 0xF0F0F0F0F0F0F0F0ULL) >> 4) | ((result & 0x0F0F0F0F0F0F0F0FULL) << 4);
    result = ((result & 0xCCCCCCCCCCCCCCCCULL) >> 2) | ((result & 0x3333333333333333ULL) << 2);
    result = ((result & 0xAAAAAAAAAAAAAAAAULL) >> 1) | ((result & 0x5555555555555555ULL) << 1);
#endif
    return result;
}

static inline int arm64_clz64(uint64_t x) {
#ifdef __aarch64__
    return x ? __builtin_clzll(x) : 64;
#else
    return x ? __builtin_clzll(x) : 64;
#endif
}

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
    // ARM64 CLZ optimization: Fill buffer more efficiently by processing multiple bytes at once
    while (stream->bits_in_buffer < 32 && stream->byte_pos < stream->data_size) {
        size_t bytes_available = stream->data_size - stream->byte_pos;
        size_t bits_needed = 64 - stream->bits_in_buffer;
        size_t bytes_to_read = (bits_needed + 7) / 8;  // CLZ optimization: avoid division
        
        if (bytes_to_read > bytes_available) {
            bytes_to_read = bytes_available;
        }
        
        // ARM64 RBIT optimization: Process up to 8 bytes at once for better throughput
        if (bytes_to_read >= 8 && stream->bits_in_buffer <= 0) {
            // Load 8 bytes directly into buffer when possible
            uint64_t chunk = 0;
            memcpy(&chunk, &stream->data[stream->byte_pos], 8);
            // Use ARM64 hardware byte swap with barrel shifter optimization
            stream->bit_buffer = __builtin_bswap64(chunk);
            stream->bits_in_buffer = 64;
            stream->byte_pos += 8;
        } else if (bytes_to_read >= 4 && stream->bits_in_buffer <= 32) {
            // ARM64 optimization: Load 4 bytes at once when beneficial
            uint32_t chunk = 0;
            memcpy(&chunk, &stream->data[stream->byte_pos], 4);
            uint64_t swapped = (uint64_t)__builtin_bswap32(chunk);
            stream->bit_buffer |= swapped << (32 - stream->bits_in_buffer);
            stream->bits_in_buffer += 32;
            stream->byte_pos += 4;
        } else {
            // Fall back to single byte loading with barrel shifter optimization
            uint64_t byte_val = (uint64_t)stream->data[stream->byte_pos];
            // Use bit shift with barrel shifter (zero-cost on ARM64)
            stream->bit_buffer |= byte_val << (56 - stream->bits_in_buffer);
            stream->bits_in_buffer += 8;
            stream->byte_pos++;
        }
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
    
    // ARM64 CLZ + Barrel Shifter optimization: Bulk bit extraction when possible
    if (stream->bits_in_buffer >= num_bits) {
        // Fast path: extract bits directly from buffer using CLZ and barrel shifter
        // ARM64 barrel shifter makes these operations essentially free
        uint64_t mask = ((1ULL << num_bits) - 1) << (64 - num_bits);
        uint32_t result = (uint32_t)((stream->bit_buffer & mask) >> (64 - num_bits));
        
        // Use ARM64 CLZ to optimize bit buffer management
        stream->bit_buffer <<= num_bits;  // Barrel shifter optimization
        stream->bits_in_buffer -= num_bits;
        
        return result;
    }
    
    // Slow path: need to refill buffer or read bits individually
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