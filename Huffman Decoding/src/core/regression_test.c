#include "regression_test.h"
#include "huffman_compress.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <sys/stat.h>

// Fixed test suite - these tests never change
const fixed_test_t FIXED_TESTS[] = {
    {
        .name = "StandardText8K",
        .filename = "fixed_tests/standard_text_8k.dat",
        .expected_size = 8192,
        .expected_ratio_min = 1.6,
        .expected_ratio_max = 2.0,
        .description = "Standard English text patterns"
    },
    {
        .name = "HighEntropy8K", 
        .filename = "fixed_tests/high_entropy_8k.dat",
        .expected_size = 8192,
        .expected_ratio_min = 0.95,
        .expected_ratio_max = 1.05,
        .description = "High entropy pseudo-random data"
    },
    {
        .name = "LowEntropy8K",
        .filename = "fixed_tests/low_entropy_8k.dat", 
        .expected_size = 8192,
        .expected_ratio_min = 3.5,
        .expected_ratio_max = 4.5,
        .description = "Low entropy repetitive data"
    },
    {
        .name = "AsciiCode8K",
        .filename = "fixed_tests/ascii_code_8k.dat",
        .expected_size = 8192, 
        .expected_ratio_min = 1.5,
        .expected_ratio_max = 2.2,
        .description = "ASCII source code patterns"
    },
    {
        .name = "BinaryMixed8K",
        .filename = "fixed_tests/binary_mixed_8k.dat",
        .expected_size = 8192,
        .expected_ratio_min = 1.1,
        .expected_ratio_max = 1.4,
        .description = "Mixed binary data with patterns"
    },
    {
        .name = "LargeText64K",
        .filename = "fixed_tests/large_text_64k.dat",
        .expected_size = 65536,
        .expected_ratio_min = 1.6,
        .expected_ratio_max = 2.0,
        .description = "Large text dataset for scalability"
    },
    {
        .name = "FrequencyTest4K",
        .filename = "fixed_tests/frequency_test_4k.dat",
        .expected_size = 4096,
        .expected_ratio_min = 1.8,
        .expected_ratio_max = 2.5,
        .description = "Specific frequency distribution test"
    }
};

const int NUM_FIXED_TESTS = sizeof(FIXED_TESTS) / sizeof(FIXED_TESTS[0]);

static uint8_t* read_test_file(const char* filename, size_t* size) {
    FILE* file = fopen(filename, "rb");
    if (!file) return NULL;
    
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

static double calculate_stddev(double* values, int count, double mean) {
    if (count < 2) return 0.0;
    
    double sum_sq_diff = 0.0;
    for (int i = 0; i < count; i++) {
        double diff = values[i] - mean;
        sum_sq_diff += diff * diff;
    }
    
    return sqrt(sum_sq_diff / (count - 1));
}

static regression_result_t run_single_regression_test(const fixed_test_t* test, int iterations) {
    regression_result_t result = {0};
    result.test_name = test->name;
    result.iterations = iterations;
    
    // Load test data
    size_t data_size;
    uint8_t* data = read_test_file(test->filename, &data_size);
    if (!data) {
        printf("ERROR: Cannot load test file: %s\n", test->filename);
        result.compression_correct = 0;
        result.decompression_correct = 0;
        return result;
    }
    
    if (data_size != test->expected_size) {
        printf("WARNING: %s size mismatch: expected %zu, got %zu\n", 
               test->name, test->expected_size, data_size);
    }
    
    result.original_size = data_size;
    
    // Arrays to store timing results for statistical analysis
    double* compress_times = malloc(iterations * sizeof(double));
    double* decompress_times = malloc(iterations * sizeof(double));
    
    benchmark_timer_t timer;
    benchmark_timer_init(&timer);
    
    size_t total_compressed_size = 0;
    int successful_iterations = 0;
    
    printf("Running %s (%d iterations)...", test->name, iterations);
    fflush(stdout);
    
    for (int i = 0; i < iterations; i++) {
        // Compression test
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
            printf(" COMPRESS_FAIL");
            continue;
        }
        
        compress_times[successful_iterations] = benchmark_timer_elapsed_ms(&timer);
        
        // Decompression test
        benchmark_timer_start(&timer);
        uint8_t* decompressed_data;
        size_t decompressed_size;
        int decompress_result = huffman_decompress_data(compressed_data, compressed_size,
                                                       symbol_table, symbol_count,
                                                       &decompressed_data, &decompressed_size,
                                                       data_size);
        benchmark_timer_stop(&timer);
        
        if (decompress_result != 0 || decompressed_size != data_size ||
            memcmp(data, decompressed_data, data_size) != 0) {
            printf(" DECOMPRESS_FAIL");
            free(compressed_data);
            free(symbol_table);
            if (decompressed_data) free(decompressed_data);
            continue;
        }
        
        decompress_times[successful_iterations] = benchmark_timer_elapsed_ms(&timer);
        total_compressed_size += compressed_size;
        successful_iterations++;
        
        // Cleanup
        free(compressed_data);
        free(symbol_table);
        free(decompressed_data);
        
        if (i % (iterations/4) == 0 && i > 0) {
            printf(".");
            fflush(stdout);
        }
    }
    
    printf(" DONE\n");
    
    if (successful_iterations == 0) {
        result.compression_correct = 0;
        result.decompression_correct = 0;
        free(data);
        free(compress_times);
        free(decompress_times);
        return result;
    }
    
    // Calculate statistics
    result.compression_correct = 1;
    result.decompression_correct = 1;
    result.compressed_size = total_compressed_size / successful_iterations;
    result.compression_ratio = (double)data_size / result.compressed_size;
    
    // Calculate timing statistics
    double total_compress_time = 0.0;
    double total_decompress_time = 0.0;
    
    for (int i = 0; i < successful_iterations; i++) {
        total_compress_time += compress_times[i];
        total_decompress_time += decompress_times[i];
    }
    
    result.compress_time_ms = total_compress_time / successful_iterations;
    result.decompress_time_ms = total_decompress_time / successful_iterations;
    
    // Calculate throughput (MB/s)
    double data_mb = data_size / 1024.0 / 1024.0;
    result.compress_throughput_mbps = data_mb / (result.compress_time_ms / 1000.0);
    result.decompress_throughput_mbps = data_mb / (result.decompress_time_ms / 1000.0);
    
    // Calculate standard deviations
    result.compress_time_stddev = calculate_stddev(compress_times, successful_iterations, result.compress_time_ms);
    result.decompress_time_stddev = calculate_stddev(decompress_times, successful_iterations, result.decompress_time_ms);
    
    free(data);
    free(compress_times);
    free(decompress_times);
    
    return result;
}

regression_suite_t* run_regression_tests(const char* version_id, int iterations) {
    regression_suite_t* suite = malloc(sizeof(regression_suite_t));
    if (!suite) return NULL;
    
    // Initialize suite
    time_t now = time(NULL);
    strftime(suite->timestamp, sizeof(suite->timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));
    strncpy(suite->version_id, version_id ? version_id : "unknown", sizeof(suite->version_id) - 1);
    suite->version_id[sizeof(suite->version_id) - 1] = '\0';
    
    suite->num_tests = NUM_FIXED_TESTS;
    suite->results = malloc(NUM_FIXED_TESTS * sizeof(regression_result_t));
    if (!suite->results) {
        free(suite);
        return NULL;
    }
    
    printf("Running Regression Test Suite\n");
    printf("============================\n");
    printf("Version: %s\n", suite->version_id);
    printf("Timestamp: %s\n", suite->timestamp);
    printf("Iterations per test: %d\n\n", iterations);
    
    // Run all tests
    for (int i = 0; i < NUM_FIXED_TESTS; i++) {
        suite->results[i] = run_single_regression_test(&FIXED_TESTS[i], iterations);
    }
    
    // Calculate overall score
    suite->total_score = calculate_suite_score(suite);
    
    return suite;
}

void free_regression_suite(regression_suite_t* suite) {
    if (!suite) return;
    if (suite->results) free(suite->results);
    free(suite);
}

double calculate_performance_score(const regression_result_t* result) {
    if (!result->compression_correct || !result->decompression_correct) {
        return 0.0;
    }
    
    // Score based on throughput (weighted towards decompression)
    double compress_score = result->compress_throughput_mbps * 0.3;
    double decompress_score = result->decompress_throughput_mbps * 0.7;
    
    return compress_score + decompress_score;
}

double calculate_suite_score(const regression_suite_t* suite) {
    double total_score = 0.0;
    int valid_tests = 0;
    
    for (int i = 0; i < suite->num_tests; i++) {
        double test_score = calculate_performance_score(&suite->results[i]);
        if (test_score > 0.0) {
            total_score += test_score;
            valid_tests++;
        }
    }
    
    return valid_tests > 0 ? total_score / valid_tests : 0.0;
}

void print_regression_results(const regression_suite_t* suite) {
    printf("\n");
    printf("=================================================================\n");
    printf("Regression Test Results - %s\n", suite->version_id);
    printf("=================================================================\n");
    printf("%-15s %8s %7s %7s %10s %10s %6s %6s\n",
           "Test", "Size", "C(ms)", "D(ms)", "C(MB/s)", "D(MB/s)", "Ratio", "Score");
    printf("-----------------------------------------------------------------\n");
    
    for (int i = 0; i < suite->num_tests; i++) {
        const regression_result_t* r = &suite->results[i];
        double score = calculate_performance_score(r);
        
        if (r->compression_correct && r->decompression_correct) {
            printf("%-15s %7.1fK %6.2f %6.2f %9.1f %9.1f %5.2f %6.0f\n",
                   r->test_name,
                   r->original_size / 1024.0,
                   r->compress_time_ms,
                   r->decompress_time_ms,
                   r->compress_throughput_mbps,
                   r->decompress_throughput_mbps,
                   r->compression_ratio,
                   score);
        } else {
            printf("%-15s %7.1fK   FAILED   FAILED      ---      ---   --- %6.0f\n",
                   r->test_name,
                   r->original_size / 1024.0,
                   score);
        }
    }
    
    printf("-----------------------------------------------------------------\n");
    printf("Overall Performance Score: %.1f\n", suite->total_score);
    printf("=================================================================\n");
}

void print_regression_summary(const regression_suite_t* suite) {
    printf("\nRegression Test Summary:\n");
    printf("  Version: %s\n", suite->version_id);
    printf("  Tests: %d\n", suite->num_tests);
    printf("  Overall Score: %.1f\n", suite->total_score);
    
    int passed = 0;
    for (int i = 0; i < suite->num_tests; i++) {
        if (suite->results[i].compression_correct && suite->results[i].decompression_correct) {
            passed++;
        }
    }
    printf("  Passed: %d/%d\n", passed, suite->num_tests);
}

int validate_test_files(void) {
    int all_exist = 1;
    
    for (int i = 0; i < NUM_FIXED_TESTS; i++) {
        struct stat st;
        if (stat(FIXED_TESTS[i].filename, &st) != 0) {
            printf("Missing test file: %s\n", FIXED_TESTS[i].filename);
            all_exist = 0;
        } else if (st.st_size != (off_t)FIXED_TESTS[i].expected_size) {
            printf("Test file size mismatch: %s (expected %zu, got %lld)\n",
                   FIXED_TESTS[i].filename, FIXED_TESTS[i].expected_size, (long long)st.st_size);
            all_exist = 0;
        }
    }
    
    return all_exist;
}

int generate_test_files_if_missing(void) {
    if (validate_test_files()) {
        return 1; // All files exist
    }
    
    printf("Some test files are missing. Generating them...\n");
    int result = system("gcc -I include/core -o generate_fixed_tests src/generate_fixed_tests.c && ./generate_fixed_tests");
    
    if (result == 0) {
        return validate_test_files();
    }
    
    return 0;
}

void save_regression_results(const regression_suite_t* suite, const char* filename) {
    FILE* f = fopen(filename, "w");
    if (!f) {
        printf("Error: Cannot save results to %s\n", filename);
        return;
    }
    
    // Save in JSON-like format for easy parsing
    fprintf(f, "{\n");
    fprintf(f, "  \"version\": \"%s\",\n", suite->version_id);
    fprintf(f, "  \"timestamp\": \"%s\",\n", suite->timestamp);
    fprintf(f, "  \"total_score\": %.2f,\n", suite->total_score);
    fprintf(f, "  \"num_tests\": %d,\n", suite->num_tests);
    fprintf(f, "  \"results\": [\n");
    
    for (int i = 0; i < suite->num_tests; i++) {
        const regression_result_t* r = &suite->results[i];
        fprintf(f, "    {\n");
        fprintf(f, "      \"name\": \"%s\",\n", r->test_name);
        fprintf(f, "      \"iterations\": %zu,\n", r->iterations);
        fprintf(f, "      \"original_size\": %zu,\n", r->original_size);
        fprintf(f, "      \"compressed_size\": %zu,\n", r->compressed_size);
        fprintf(f, "      \"compression_ratio\": %.3f,\n", r->compression_ratio);
        fprintf(f, "      \"compress_time_ms\": %.3f,\n", r->compress_time_ms);
        fprintf(f, "      \"decompress_time_ms\": %.3f,\n", r->decompress_time_ms);
        fprintf(f, "      \"compress_throughput_mbps\": %.2f,\n", r->compress_throughput_mbps);
        fprintf(f, "      \"decompress_throughput_mbps\": %.2f,\n", r->decompress_throughput_mbps);
        fprintf(f, "      \"compress_time_stddev\": %.3f,\n", r->compress_time_stddev);
        fprintf(f, "      \"decompress_time_stddev\": %.3f,\n", r->decompress_time_stddev);
        fprintf(f, "      \"compression_correct\": %s,\n", r->compression_correct ? "true" : "false");
        fprintf(f, "      \"decompression_correct\": %s,\n", r->decompression_correct ? "true" : "false");
        fprintf(f, "      \"score\": %.2f\n", calculate_performance_score(r));
        fprintf(f, "    }%s\n", (i < suite->num_tests - 1) ? "," : "");
    }
    
    fprintf(f, "  ]\n");
    fprintf(f, "}\n");
    
    fclose(f);
}