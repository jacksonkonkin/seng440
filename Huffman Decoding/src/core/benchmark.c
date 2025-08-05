#include "benchmark.h"
#include "huffman_compress.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/sysctl.h>

static mach_timebase_info_data_t timebase_info;

void benchmark_timer_init(benchmark_timer_t* timer) {
    if (timebase_info.denom == 0) {
        mach_timebase_info(&timebase_info);
    }
    timer->timebase_factor = (double)timebase_info.numer / timebase_info.denom / 1e6; // Convert to milliseconds
    timer->start_time = 0;
    timer->end_time = 0;
}

void benchmark_timer_start(benchmark_timer_t* timer) {
    timer->start_time = mach_absolute_time();
}

void benchmark_timer_stop(benchmark_timer_t* timer) {
    timer->end_time = mach_absolute_time();
}

double benchmark_timer_elapsed_ms(const benchmark_timer_t* timer) {
    return (timer->end_time - timer->start_time) * timer->timebase_factor;
}

double benchmark_timer_elapsed_us(const benchmark_timer_t* timer) {
    return benchmark_timer_elapsed_ms(timer) * 1000.0;
}

benchmark_result_t benchmark_compression(const char* test_name, 
                                       const uint8_t* data, size_t data_size,
                                       int iterations) {
    benchmark_result_t result = {0};
    result.name = test_name;
    result.data_size = data_size;
    
    benchmark_timer_t timer;
    benchmark_timer_init(&timer);
    
    // Compression benchmarking
    double total_compress_time = 0.0;
    double min_compress_time = 1e9;
    double max_compress_time = 0.0;
    size_t total_compressed_size = 0;
    
    printf("Running %s compression benchmark (%d iterations)...\n", test_name, iterations);
    
    for (int i = 0; i < iterations; i++) {
        uint8_t* compressed_data;
        size_t compressed_size;
        symbol_info_t* symbol_table;
        size_t symbol_count;
        
        benchmark_timer_start(&timer);
        int compress_result = huffman_compress_data(data, data_size, 
                                                   &compressed_data, &compressed_size,
                                                   &symbol_table, &symbol_count);
        benchmark_timer_stop(&timer);
        
        if (compress_result != 0) {
            printf("Compression failed on iteration %d\n", i);
            continue;
        }
        
        double elapsed = benchmark_timer_elapsed_ms(&timer);
        total_compress_time += elapsed;
        if (elapsed < min_compress_time) min_compress_time = elapsed;
        if (elapsed > max_compress_time) max_compress_time = elapsed;
        
        // Test decompression for this iteration
        benchmark_timer_start(&timer);
        uint8_t* decompressed_data;
        size_t decompressed_size;
        int decompress_result = huffman_decompress_data(compressed_data, compressed_size,
                                                       symbol_table, symbol_count,
                                                       &decompressed_data, &decompressed_size,
                                                       data_size);
        benchmark_timer_stop(&timer);
        
        if (decompress_result != 0 || decompressed_size != data_size) {
            printf("Decompression failed on iteration %d\n", i);
            free(compressed_data);
            free(symbol_table);
            continue;
        }
        
        double decompress_elapsed = benchmark_timer_elapsed_ms(&timer);
        result.decompress_stats.total_time += decompress_elapsed;
        if (i == 0) {
            result.decompress_stats.min_time = decompress_elapsed;
            result.decompress_stats.max_time = decompress_elapsed;
        } else {
            if (decompress_elapsed < result.decompress_stats.min_time) 
                result.decompress_stats.min_time = decompress_elapsed;
            if (decompress_elapsed > result.decompress_stats.max_time) 
                result.decompress_stats.max_time = decompress_elapsed;
        }
        
        total_compressed_size += compressed_size;
        
        free(compressed_data);
        free(symbol_table);
        free(decompressed_data);
    }
    
    // Calculate compression stats
    result.compress_stats.iterations = iterations;
    result.compress_stats.total_time = total_compress_time;
    result.compress_stats.avg_time = total_compress_time / iterations;
    result.compress_stats.min_time = min_compress_time;
    result.compress_stats.max_time = max_compress_time;
    result.compress_stats.throughput_mbps = (data_size / 1024.0 / 1024.0) / (result.compress_stats.avg_time / 1000.0);
    
    // Calculate decompression stats
    result.decompress_stats.iterations = iterations;
    result.decompress_stats.avg_time = result.decompress_stats.total_time / iterations;
    result.decompress_stats.throughput_mbps = (data_size / 1024.0 / 1024.0) / (result.decompress_stats.avg_time / 1000.0);
    
    // Compression ratio
    result.compressed_size = total_compressed_size / iterations;
    result.compression_ratio = (double)data_size / result.compressed_size;
    
    return result;
}

void benchmark_print_header(void) {
    printf("\n");
    printf("=================================================================\n");
    printf("M4-Optimized Huffman Compression Benchmark Results\n");
    printf("=================================================================\n");
    printf("%-20s %8s %10s %10s %10s %8s %10s\n", 
           "Test", "Size", "Comp(ms)", "Decomp(ms)", "Comp MB/s", "Ratio", "Space");
    printf("-----------------------------------------------------------------\n");
}

void benchmark_print_result(const benchmark_result_t* result) {
    double space_savings = (1.0 - 1.0/result->compression_ratio) * 100.0;
    
    printf("%-20s %8.1fK %9.2f %10.2f %9.1f %7.2f:1 %8.1f%%\n",
           result->name,
           result->data_size / 1024.0,
           result->compress_stats.avg_time,
           result->decompress_stats.avg_time,
           result->compress_stats.throughput_mbps,
           result->compression_ratio,
           space_savings);
}

uint8_t* generate_random_data(size_t size) {
    uint8_t* data = malloc(size);
    if (!data) return NULL;
    
    for (size_t i = 0; i < size; i++) {
        data[i] = rand() % 256;
    }
    return data;
}

uint8_t* generate_text_data(size_t size) {
    uint8_t* data = malloc(size);
    if (!data) return NULL;
    
    const char* sample_text = "The quick brown fox jumps over the lazy dog. "
                             "This is a sample text for compression testing. "
                             "It contains common English words and patterns. ";
    size_t sample_len = strlen(sample_text);
    
    for (size_t i = 0; i < size; i++) {
        data[i] = sample_text[i % sample_len];
    }
    return data;
}

uint8_t* generate_repetitive_data(size_t size) {
    uint8_t* data = malloc(size);
    if (!data) return NULL;
    
    const uint8_t pattern[] = {0xAA, 0xBB, 0xCC, 0xDD};
    
    for (size_t i = 0; i < size; i++) {
        data[i] = pattern[i % 4];
    }
    return data;
}

uint8_t* generate_binary_data(size_t size) {
    uint8_t* data = malloc(size);
    if (!data) return NULL;
    
    // Generate binary-like data with specific byte patterns
    for (size_t i = 0; i < size; i++) {
        if (i % 8 == 0) data[i] = 0x00;
        else if (i % 8 == 7) data[i] = 0xFF;
        else data[i] = (i * 17) % 256;
    }
    return data;
}

void print_system_info(void) {
    printf("\nSystem Information:\n");
    
    // Get system info
    size_t size = sizeof(int);
    int mib[2];
    
    // CPU info
    mib[0] = CTL_HW;
    mib[1] = HW_NCPU;
    int ncpu;
    size = sizeof(ncpu);
    if (sysctl(mib, 2, &ncpu, &size, NULL, 0) == 0) {
        printf("  CPU Cores: %d\n", ncpu);
    }
    
    // Memory info
    mib[1] = HW_MEMSIZE;
    uint64_t memsize;
    size = sizeof(memsize);
    if (sysctl(mib, 2, &memsize, &size, NULL, 0) == 0) {
        printf("  Memory: %.1f GB\n", memsize / 1024.0 / 1024.0 / 1024.0);
    }
    
    // CPU frequency (approximation for Apple Silicon)
    printf("  Architecture: Apple Silicon ARM64\n");
    printf("  Optimization: M4-specific with NEON SIMD\n");
    
    printf("\n");
}

void print_cpu_info(void) {
    char cpu_brand[256];
    size_t size = sizeof(cpu_brand);
    
    if (sysctlbyname("machdep.cpu.brand_string", cpu_brand, &size, NULL, 0) == 0) {
        printf("  CPU: %s\n", cpu_brand);
    }
}