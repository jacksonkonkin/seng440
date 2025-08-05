#ifndef REGRESSION_TEST_H
#define REGRESSION_TEST_H

#include <stdint.h>
#include <stddef.h>
#include "benchmark.h"

// Fixed test definitions for consistent benchmarking
typedef struct {
    const char* name;
    const char* filename;
    size_t expected_size;
    double expected_ratio_min;
    double expected_ratio_max;
    const char* description;
} fixed_test_t;

typedef struct {
    const char* test_name;
    size_t iterations;
    
    // Timing results
    double compress_time_ms;
    double decompress_time_ms;
    double compress_throughput_mbps;
    double decompress_throughput_mbps;
    
    // Compression results  
    size_t original_size;
    size_t compressed_size;
    double compression_ratio;
    
    // Validation
    int compression_correct;
    int decompression_correct;
    
    // Statistical info
    double compress_time_stddev;
    double decompress_time_stddev;
} regression_result_t;

typedef struct {
    char timestamp[32];
    char version_id[64];
    int num_tests;
    regression_result_t* results;
    double total_score;
} regression_suite_t;

// Fixed test suite
extern const fixed_test_t FIXED_TESTS[];
extern const int NUM_FIXED_TESTS;

// Test execution
regression_suite_t* run_regression_tests(const char* version_id, int iterations);
void free_regression_suite(regression_suite_t* suite);

// Results analysis
void print_regression_results(const regression_suite_t* suite);
void print_regression_summary(const regression_suite_t* suite);
void save_regression_results(const regression_suite_t* suite, const char* filename);

// Comparison functions
int compare_regression_suites(const regression_suite_t* baseline, 
                             const regression_suite_t* current);
void print_performance_diff(const regression_suite_t* baseline,
                           const regression_suite_t* current);

// Performance scoring
double calculate_performance_score(const regression_result_t* result);
double calculate_suite_score(const regression_suite_t* suite);

// Validation functions
int validate_test_files(void);
int generate_test_files_if_missing(void);

#endif