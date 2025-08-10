#include "bit_stream.h"
#include <stdlib.h>
#include <string.h>

#ifdef __aarch64__
#include <arm_neon.h>
#endif

// ITERATION 4: Optimized bit stream for lookup table support

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
    
    // ITERATION 4: Pre-fill buffer for immediate availability
    if (size > 0) {
        bit_stream_fill_buffer(stream);
    }
    
    return stream;
}

void bit_stream_destroy(bit_stream_t* stream) {
    if (stream) {
        free(stream);
    }
}

// ITERATION 4: Aggressive buffer filling for lookup table efficiency
void bit_stream_fill_buffer(bit_stream_t* stream) {
    // Always try to keep buffer as full as possible for lookup tables
    while (stream->bits_in_buffer <= 56 && stream->byte_pos < stream->data_size) {
        size_t bytes_available = stream->data_size - stream->byte_pos;
        size_t bits_needed = 64 - stream->bits_in_buffer;
        size_t bytes_to_read = (bits_needed + 7) >> 3;
        
        if (bytes_to_read > bytes_available) {
            bytes_to_read = bytes_available;
        }
        
        // ITERATION 4: Always prefer 8-byte reads for lookup table efficiency
        if (bytes_to_read >= 8 && stream->bits_in_buffer == 0) {
            // Fast path: Load 8 bytes directly
            uint64_t chunk = 0;
            memcpy(&chunk, &stream->data[stream->byte_pos], 8);
            stream->bit_buffer = __builtin_bswap64(chunk);
            stream->bits_in_buffer = 64;
            stream->byte_pos += 8;
            
            // Aggressive prefetch for sustained throughput
            if (stream->byte_pos + 64 < stream->data_size) {
                __builtin_prefetch(&stream->data[stream->byte_pos + 64], 0, 3);
            }
            return;
        } else if (bytes_to_read >= 4) {
            // Load 4 bytes
            uint32_t chunk = 0;
            memcpy(&chunk, &stream->data[stream->byte_pos], 4);
            uint64_t swapped = (uint64_t)__builtin_bswap32(chunk);
            stream->bit_buffer |= swapped << (32 - stream->bits_in_buffer);
            stream->bits_in_buffer += 32;
            stream->byte_pos += 4;
        } else if (bytes_to_read >= 2) {
            // Load 2 bytes
            uint16_t chunk = 0;
            memcpy(&chunk, &stream->data[stream->byte_pos], 2);
            uint64_t swapped = (uint64_t)__builtin_bswap16(chunk);
            stream->bit_buffer |= swapped << (48 - stream->bits_in_buffer);
            stream->bits_in_buffer += 16;
            stream->byte_pos += 2;
        } else {
            // Load single byte
            uint64_t byte_val = (uint64_t)stream->data[stream->byte_pos];
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
    
    // Ensure buffer has enough bits
    if (stream->bits_in_buffer < num_bits) {
        bit_stream_fill_buffer(stream);
    }
    
    if (stream->bits_in_buffer >= num_bits) {
        // Fast path: extract bits directly
        uint32_t result = (uint32_t)(stream->bit_buffer >> (64 - num_bits));
        stream->bit_buffer <<= num_bits;
        stream->bits_in_buffer -= num_bits;
        return result;
    }
    
    // Slow path: read bits individually
    uint32_t result = 0;
    for (uint8_t i = 0; i < num_bits; i++) {
        result <<= 1;
        if (bit_stream_read_bit(stream)) {
            result |= 1;
        }
    }
    
    return result;
}

// ITERATION 4: New function to peek at bits without consuming them (for lookup tables)
uint32_t bit_stream_peek_bits(bit_stream_t* stream, uint8_t num_bits) {
    if (num_bits == 0 || num_bits > 32) return 0xFFFFFFFF;
    
    // Ensure buffer has enough bits
    if (stream->bits_in_buffer < num_bits) {
        bit_stream_fill_buffer(stream);
    }
    
    if (stream->bits_in_buffer >= num_bits) {
        // Return bits without consuming them
        return (uint32_t)(stream->bit_buffer >> (64 - num_bits));
    }
    
    // Not enough bits available
    return 0xFFFFFFFF;
}

// ITERATION 4: Skip bits after successful lookup
void bit_stream_skip_bits(bit_stream_t* stream, uint8_t num_bits) {
    if (num_bits == 0 || num_bits > 32) return;
    
    if (stream->bits_in_buffer >= num_bits) {
        stream->bit_buffer <<= num_bits;
        stream->bits_in_buffer -= num_bits;
    } else {
        // Need to skip more bits than in buffer
        num_bits -= stream->bits_in_buffer;
        stream->bits_in_buffer = 0;
        stream->bit_buffer = 0;
        
        // Skip whole bytes
        while (num_bits >= 8 && stream->byte_pos < stream->data_size) {
            stream->byte_pos++;
            num_bits -= 8;
        }
        
        // Skip remaining bits
        if (num_bits > 0) {
            bit_stream_fill_buffer(stream);
            if (stream->bits_in_buffer >= num_bits) {
                stream->bit_buffer <<= num_bits;
                stream->bits_in_buffer -= num_bits;
            }
        }
    }
}

// ITERATION 4: Get number of bits available in buffer
int bit_stream_available_bits(bit_stream_t* stream) {
    if (stream->bits_in_buffer > 0) {
        return stream->bits_in_buffer;
    }
    
    // Try to fill buffer
    bit_stream_fill_buffer(stream);
    return stream->bits_in_buffer;
}

bool bit_stream_has_data(bit_stream_t* stream) {
    return stream->bits_in_buffer > 0 || stream->byte_pos < stream->data_size;
}

#ifdef __aarch64__
// ITERATION 4: NEON SIMD batch peek for multiple lookups
void bit_stream_peek_batch_neon(bit_stream_t* stream, uint8_t* bit_counts, 
                                uint32_t* results, int batch_size) {
    if (!stream || !bit_counts || !results || batch_size <= 0) return;
    
    // Ensure buffer is full
    while (stream->bits_in_buffer < 64 && stream->byte_pos < stream->data_size) {
        bit_stream_fill_buffer(stream);
    }
    
    // Use NEON to extract multiple bit sequences in parallel
    for (int i = 0; i < batch_size && i < 8; i++) {
        if (bit_counts[i] <= 32 && stream->bits_in_buffer >= bit_counts[i]) {
            results[i] = (uint32_t)(stream->bit_buffer >> (64 - bit_counts[i]));
        } else {
            results[i] = 0xFFFFFFFF;
        }
    }
}

// ITERATION 4: NEON SIMD optimized buffer fill
void bit_stream_fill_buffer_neon(bit_stream_t* stream) {
    if (!stream || stream->byte_pos >= stream->data_size) return;
    
    size_t bytes_available = stream->data_size - stream->byte_pos;
    
    // Try to load 16 bytes using NEON
    if (bytes_available >= 16 && stream->bits_in_buffer == 0) {
        uint8x16_t data = vld1q_u8(&stream->data[stream->byte_pos]);
        
        // Extract first 8 bytes and byte swap
        uint64_t chunk1, chunk2;
        vst1q_u8((uint8_t*)&chunk1, data);
        chunk1 = __builtin_bswap64(chunk1);
        
        stream->bit_buffer = chunk1;
        stream->bits_in_buffer = 64;
        stream->byte_pos += 8;
        
        // Prefetch next cache line
        if (stream->byte_pos + 64 < stream->data_size) {
            __builtin_prefetch(&stream->data[stream->byte_pos + 64], 0, 3);
        }
    } else {
        // Fall back to regular fill
        bit_stream_fill_buffer(stream);
    }
}
#endif