#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <stdint.h>
#include <stddef.h>
#include <sys/time.h>
#include <mach/mach_time.h>

// High-precision timing utilities
typedef struct {
    uint64_t start_time;
    uint64_t end_time;
    double timebase_factor;
} benchmark_timer_t;

typedef struct {
    double min_time;
    double max_time;
    double avg_time;
    double total_time;
    size_t iterations;
    double throughput_mbps;  // MB/s
} benchmark_stats_t;

typedef struct {
    const char* name;
    size_t data_size;
    benchmark_stats_t compress_stats;
    benchmark_stats_t decompress_stats;
    double compression_ratio;
    size_t compressed_size;
} benchmark_result_t;

// Timer functions
void benchmark_timer_init(benchmark_timer_t* timer);
void benchmark_timer_start(benchmark_timer_t* timer);
void benchmark_timer_stop(benchmark_timer_t* timer);
double benchmark_timer_elapsed_ms(const benchmark_timer_t* timer);
double benchmark_timer_elapsed_us(const benchmark_timer_t* timer);

// Benchmarking functions
benchmark_result_t benchmark_compression(const char* test_name, 
                                       const uint8_t* data, size_t data_size,
                                       int iterations);

void benchmark_print_result(const benchmark_result_t* result);
void benchmark_print_header(void);

// Test data generation
uint8_t* generate_random_data(size_t size);
uint8_t* generate_text_data(size_t size);
uint8_t* generate_repetitive_data(size_t size);
uint8_t* generate_binary_data(size_t size);

// System info
void print_system_info(void);
void print_cpu_info(void);

#endif