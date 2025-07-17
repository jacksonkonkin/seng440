//
//  test_setup.c
//  Huffman Decoding M4 Setup Test
//
//  Created by Jackson Konkin on 2025-07-17.
//  Basic test to verify ARM64 optimizations and build system
//

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <arm_neon.h>

// Test ARM64 hardware instructions
void test_arm64_instructions() {
    printf("Testing ARM64 Instructions:\n");
    printf("==========================\n");
    
    // Test CLZ (Count Leading Zeros) - critical for Huffman decoding
    uint64_t test_val = 0x0000000000FF0000ULL;
    uint64_t leading_zeros = __builtin_clzll(test_val);
    printf("CLZ test: 0x%016llx has %llu leading zeros\n", test_val, leading_zeros);
    
    // Test RBIT (Reverse Bits) using inline assembly
    uint32_t original = 0x12345678;
    uint32_t reversed;
    __asm__("rbit %w0, %w1" : "=r" (reversed) : "r" (original));
    printf("RBIT test: 0x%08x reversed = 0x%08x\n", original, reversed);
    
    // Test bit manipulation
    uint64_t bit_field = 0xABCDEF0123456789ULL;
    uint64_t extracted = (bit_field >> 16) & 0xFFFF;
    printf("Bit manipulation: extracted 0x%04llx from 0x%016llx\n", extracted, bit_field);
    
    printf("\n");
}

// Test NEON SIMD operations
void test_neon_simd() {
    printf("Testing NEON SIMD Operations:\n");
    printf("=============================\n");
    
    // Test 128-bit vector operations
    uint8_t data[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    uint8x16_t vec = vld1q_u8(data);
    
    // Add 1 to each element
    uint8x16_t ones = vdupq_n_u8(1);
    uint8x16_t result = vaddq_u8(vec, ones);
    
    // Store result
    uint8_t output[16];
    vst1q_u8(output, result);
    
    printf("NEON vector addition test:\n");
    printf("Input:  ");
    for (int i = 0; i < 16; i++) {
        printf("%2d ", data[i]);
    }
    printf("\nOutput: ");
    for (int i = 0; i < 16; i++) {
        printf("%2d ", output[i]);
    }
    printf("\n");
    
    // Test parallel comparison (useful for Huffman symbol matching)
    uint8_t search_val = 5;
    uint8x16_t search_vec = vdupq_n_u8(search_val);
    uint8x16_t mask = vceqq_u8(vec, search_vec);
    
    uint8_t mask_result[16];
    vst1q_u8(mask_result, mask);
    
    printf("Parallel comparison (searching for %d):\n", search_val);
    printf("Mask:   ");
    for (int i = 0; i < 16; i++) {
        printf("%2d ", mask_result[i] ? 1 : 0);
    }
    printf("\n\n");
}

// Test cache-aligned memory allocation
void test_cache_alignment() {
    printf("Testing Cache Alignment:\n");
    printf("========================\n");
    
    // Allocate cache-aligned memory (64-byte alignment for M4)
    size_t alignment = 64;
    size_t size = 1024;
    
    void* aligned_mem = aligned_alloc(alignment, size);
    if (aligned_mem) {
        printf("Successfully allocated %zu bytes with %zu-byte alignment\n", size, alignment);
        printf("Memory address: %p\n", aligned_mem);
        printf("Address mod 64: %lu (should be 0)\n", (uintptr_t)aligned_mem % 64);
        
        // Test memory access
        uint64_t* ptr = (uint64_t*)aligned_mem;
        *ptr = 0xDEADBEEFCAFEBABEULL;
        printf("Memory test: wrote 0x%016llx, read 0x%016llx\n", 0xDEADBEEFCAFEBABEULL, *ptr);
        
        free(aligned_mem);
    } else {
        printf("Failed to allocate aligned memory\n");
    }
    
    printf("\n");
}

// Test compiler optimizations
void test_compiler_optimizations() {
    printf("Testing Compiler Optimizations:\n");
    printf("===============================\n");
    
    // Check if compiler generates optimal code for common patterns
    volatile uint64_t test_data = 0x123456789ABCDEF0ULL;
    
    // This should use CLZ instruction
    int leading_zeros = __builtin_clzll(test_data);
    printf("Leading zeros in 0x%016llx: %d\n", test_data, leading_zeros);
    
    // This should use efficient bit manipulation
    uint64_t shifted = test_data >> 8;
    uint64_t masked = shifted & 0xFF;
    printf("Bit manipulation result: 0x%02llx\n", masked);
    
    // Test branch prediction hint
    if (__builtin_expect(test_data != 0, 1)) {
        printf("Branch prediction hint working\n");
    }
    
    printf("\n");
}

// Test system information
void test_system_info() {
    printf("System Information:\n");
    printf("==================\n");
    
    #ifdef __aarch64__
        printf("Architecture: ARM64/AArch64 ✓\n");
    #else
        printf("Architecture: Not ARM64 ✗\n");
    #endif
    
    #ifdef __APPLE__
        printf("Platform: Apple Silicon ✓\n");
    #else
        printf("Platform: Not Apple ✗\n");
    #endif
    
    #ifdef __ARM_NEON
        printf("NEON Support: Available ✓\n");
    #else
        printf("NEON Support: Not Available ✗\n");
    #endif
    
    #ifdef __ARM_FEATURE_CLZ
        printf("CLZ Instruction: Available ✓\n");
    #else
        printf("CLZ Instruction: Not Available ✗\n");
    #endif
    
    printf("Compiler: %s\n", __VERSION__);
    printf("Optimization: ");
    #ifdef __OPTIMIZE__
        printf("Enabled ✓\n");
    #else
        printf("Disabled ✗\n");
    #endif
    
    printf("\n");
}

int main() {
    printf("M4-Optimized Huffman Decoder - Setup Test\n");
    printf("==========================================\n\n");
    
    test_system_info();
    test_arm64_instructions();
    test_neon_simd();
    test_cache_alignment();
    test_compiler_optimizations();
    
    printf("Setup test completed successfully!\n");
    printf("Ready to proceed with Huffman decoder implementation.\n");
    
    return 0;
}