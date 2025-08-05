#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "benchmark.h"

typedef struct {
    int iterations;
    int verbose;
    int run_all;
    const char* specific_test;
    const char* input_file;
} benchmark_config_t;

void print_usage(const char* program_name) {
    printf("M4-Optimized Huffman Compression Benchmark Suite\n");
    printf("Usage: %s [OPTIONS]\n\n", program_name);
    printf("Options:\n");
    printf("  -i, --iterations N    Number of iterations per test (default: 10)\n");
    printf("  -v, --verbose         Enable verbose output\n");
    printf("  -a, --all             Run all benchmark tests (default)\n");
    printf("  -t, --test NAME       Run specific test (text|random|repetitive|binary|file)\n");
    printf("  -f, --file PATH       Benchmark specific file\n");
    printf("  -h, --help            Show this help message\n\n");
    printf("Examples:\n");
    printf("  %s -i 20 -a           # Run all tests with 20 iterations\n", program_name);
    printf("  %s -t text -v         # Run only text test with verbose output\n", program_name);
    printf("  %s -f sample.txt      # Benchmark specific file\n", program_name);
}

uint8_t* read_file_for_benchmark(const char* filename, size_t* size) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("Error: Cannot open file '%s'\n", filename);
        return NULL;
    }
    
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    if (file_size <= 0) {
        fclose(file);
        return NULL;
    }
    
    uint8_t* data = malloc(file_size);
    if (!data) {
        fclose(file);
        return NULL;
    }
    
    size_t read_size = fread(data, 1, file_size, file);
    fclose(file);
    
    if (read_size != (size_t)file_size) {
        free(data);
        return NULL;
    }
    
    *size = file_size;
    return data;
}

void run_synthetic_benchmarks(const benchmark_config_t* config) {
    const size_t test_sizes[] = {1024, 4096, 16384, 65536, 262144}; // 1KB to 256KB
    const int num_sizes = sizeof(test_sizes) / sizeof(test_sizes[0]);
    
    for (int size_idx = 0; size_idx < num_sizes; size_idx++) {
        size_t size = test_sizes[size_idx];
        
        // Text data benchmark
        if (config->run_all || (config->specific_test && strcmp(config->specific_test, "text") == 0)) {
            char test_name[64];
            snprintf(test_name, sizeof(test_name), "Text-%dK", (int)(size / 1024));
            
            uint8_t* text_data = generate_text_data(size);
            if (text_data) {
                benchmark_result_t result = benchmark_compression(test_name, text_data, size, config->iterations);
                benchmark_print_result(&result);
                free(text_data);
            }
        }
        
        // Random data benchmark
        if (config->run_all || (config->specific_test && strcmp(config->specific_test, "random") == 0)) {
            char test_name[64];
            snprintf(test_name, sizeof(test_name), "Random-%dK", (int)(size / 1024));
            
            uint8_t* random_data = generate_random_data(size);
            if (random_data) {
                benchmark_result_t result = benchmark_compression(test_name, random_data, size, config->iterations);
                benchmark_print_result(&result);
                free(random_data);
            }
        }
        
        // Repetitive data benchmark
        if (config->run_all || (config->specific_test && strcmp(config->specific_test, "repetitive") == 0)) {
            char test_name[64];
            snprintf(test_name, sizeof(test_name), "Repetitive-%dK", (int)(size / 1024));
            
            uint8_t* rep_data = generate_repetitive_data(size);
            if (rep_data) {
                benchmark_result_t result = benchmark_compression(test_name, rep_data, size, config->iterations);
                benchmark_print_result(&result);
                free(rep_data);
            }
        }
        
        // Binary data benchmark
        if (config->run_all || (config->specific_test && strcmp(config->specific_test, "binary") == 0)) {
            char test_name[64];
            snprintf(test_name, sizeof(test_name), "Binary-%dK", (int)(size / 1024));
            
            uint8_t* binary_data = generate_binary_data(size);
            if (binary_data) {
                benchmark_result_t result = benchmark_compression(test_name, binary_data, size, config->iterations);
                benchmark_print_result(&result);
                free(binary_data);
            }
        }
    }
}

void run_file_benchmark(const benchmark_config_t* config) {
    size_t file_size;
    uint8_t* file_data = read_file_for_benchmark(config->input_file, &file_size);
    
    if (!file_data) {
        printf("Failed to read file: %s\n", config->input_file);
        return;
    }
    
    printf("Benchmarking file: %s (%.1f KB)\n", config->input_file, file_size / 1024.0);
    
    benchmark_result_t result = benchmark_compression(config->input_file, file_data, file_size, config->iterations);
    benchmark_print_result(&result);
    
    if (config->verbose) {
        printf("\nDetailed results for %s:\n", config->input_file);
        printf("  Original size: %zu bytes\n", file_size);
        printf("  Compressed size: %zu bytes\n", result.compressed_size);
        printf("  Compression time (min/avg/max): %.2f/%.2f/%.2f ms\n", 
               result.compress_stats.min_time, result.compress_stats.avg_time, result.compress_stats.max_time);
        printf("  Decompression time (min/avg/max): %.2f/%.2f/%.2f ms\n",
               result.decompress_stats.min_time, result.decompress_stats.avg_time, result.decompress_stats.max_time);
        printf("  Compression throughput: %.1f MB/s\n", result.compress_stats.throughput_mbps);
        printf("  Decompression throughput: %.1f MB/s\n", result.decompress_stats.throughput_mbps);
    }
    
    free(file_data);
}

int main(int argc, char* argv[]) {
    benchmark_config_t config = {
        .iterations = 10,
        .verbose = 0,
        .run_all = 1,
        .specific_test = NULL,
        .input_file = NULL
    };
    
    static struct option long_options[] = {
        {"iterations", required_argument, 0, 'i'},
        {"verbose",    no_argument,       0, 'v'},
        {"all",        no_argument,       0, 'a'},
        {"test",       required_argument, 0, 't'},
        {"file",       required_argument, 0, 'f'},
        {"help",       no_argument,       0, 'h'},
        {0, 0, 0, 0}
    };
    
    int c;
    while ((c = getopt_long(argc, argv, "i:vat:f:h", long_options, NULL)) != -1) {
        switch (c) {
            case 'i':
                config.iterations = atoi(optarg);
                if (config.iterations < 1) {
                    printf("Error: Iterations must be at least 1\n");
                    return 1;
                }
                break;
            case 'v':
                config.verbose = 1;
                break;
            case 'a':
                config.run_all = 1;
                config.specific_test = NULL;
                break;
            case 't':
                config.run_all = 0;
                config.specific_test = optarg;
                break;
            case 'f':
                config.run_all = 0;
                config.input_file = optarg;
                break;
            case 'h':
                print_usage(argv[0]);
                return 0;
            case '?':
                print_usage(argv[0]);
                return 1;
        }
    }
    
    // Print system information
    print_system_info();
    
    // Print benchmark header
    benchmark_print_header();
    
    // Run benchmarks
    if (config.input_file) {
        run_file_benchmark(&config);
    } else {
        run_synthetic_benchmarks(&config);
    }
    
    printf("-----------------------------------------------------------------\n");
    printf("Benchmark completed with %d iterations per test\n", config.iterations);
    printf("Higher MB/s values indicate better performance\n");
    printf("=================================================================\n\n");
    
    return 0;
}