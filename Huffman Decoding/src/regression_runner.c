#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "regression_test.h"

void print_usage(const char* program_name) {
    printf("Huffman Compression Regression Test Suite\n");
    printf("Usage: %s [OPTIONS]\n\n", program_name);
    printf("Options:\n");
    printf("  -v, --version ID      Version identifier for this run\n");
    printf("  -i, --iterations N    Number of iterations per test (default: 20)\n");
    printf("  -o, --output FILE     Save results to file\n");
    printf("  -c, --compare FILE    Compare with previous results\n");
    printf("  -g, --generate        Generate test files if missing\n");
    printf("  -V, --validate        Validate test files only\n");
    printf("  -s, --summary         Show summary only (less verbose)\n");
    printf("  -h, --help            Show this help message\n\n");
    printf("Examples:\n");
    printf("  %s -v \"baseline\" -i 50 -o baseline.json\n", program_name);
    printf("  %s -v \"optimized\" -c baseline.json\n", program_name);
    printf("  %s -g                  # Generate missing test files\n", program_name);
}

int main(int argc, char* argv[]) {
    const char* version_id = "current";
    int iterations = 20;
    const char* output_file = NULL;
    const char* compare_file = NULL;
    int generate_files = 0;
    int validate_only = 0;
    int summary_only = 0;
    
    static struct option long_options[] = {
        {"version",    required_argument, 0, 'v'},
        {"iterations", required_argument, 0, 'i'},
        {"output",     required_argument, 0, 'o'},
        {"compare",    required_argument, 0, 'c'},
        {"generate",   no_argument,       0, 'g'},
        {"validate",   no_argument,       0, 'V'},
        {"summary",    no_argument,       0, 's'},
        {"help",       no_argument,       0, 'h'},
        {0, 0, 0, 0}
    };
    
    int c;
    while ((c = getopt_long(argc, argv, "v:i:o:c:gVsh", long_options, NULL)) != -1) {
        switch (c) {
            case 'v':
                version_id = optarg;
                break;
            case 'i':
                iterations = atoi(optarg);
                if (iterations < 1) {
                    printf("Error: Iterations must be at least 1\n");
                    return 1;
                }
                break;
            case 'o':
                output_file = optarg;
                break;
            case 'c':
                compare_file = optarg;
                break;
            case 'g':
                generate_files = 1;
                break;
            case 'V':
                validate_only = 1;
                break;
            case 's':
                summary_only = 1;
                break;
            case 'h':
                print_usage(argv[0]);
                return 0;
            case '?':
                print_usage(argv[0]);
                return 1;
        }
    }
    
    // Handle generate files option
    if (generate_files) {
        printf("Generating fixed test files...\n");
        if (generate_test_files_if_missing()) {
            printf("Test files generated successfully.\n");
            return 0;
        } else {
            printf("Failed to generate test files.\n");
            return 1;
        }
    }
    
    // Handle validate only option
    if (validate_only) {
        printf("Validating test files...\n");
        if (validate_test_files()) {
            printf("All test files are valid.\n");
            return 0;
        } else {
            printf("Some test files are missing or invalid.\n");
            printf("Run with -g to generate them.\n");
            return 1;
        }
    }
    
    // Ensure test files exist
    if (!validate_test_files()) {
        printf("Test files are missing or invalid.\n");
        printf("Attempting to generate them...\n");
        if (!generate_test_files_if_missing()) {
            printf("Failed to generate test files. Cannot proceed.\n");
            return 1;
        }
        printf("Test files generated successfully.\n\n");
    }
    
    // Run regression tests
    regression_suite_t* suite = run_regression_tests(version_id, iterations);
    if (!suite) {
        printf("Failed to run regression tests.\n");
        return 1;
    }
    
    // Print results
    if (summary_only) {
        print_regression_summary(suite);
    } else {
        print_regression_results(suite);
    }
    
    // Save results if requested
    if (output_file) {
        save_regression_results(suite, output_file);
        printf("\nResults saved to: %s\n", output_file);
    }
    
    // Compare with previous results if requested
    if (compare_file) {
        printf("\nComparison with %s:\n", compare_file);
        printf("(Comparison functionality to be implemented)\n");
        // TODO: Load and compare with previous results
    }
    
    // Print final summary
    printf("\n");
    printf("Summary:\n");
    printf("  Version: %s\n", version_id);
    printf("  Overall Score: %.1f\n", suite->total_score);
    printf("  Iterations: %d per test\n", iterations);
    
    int failed_tests = 0;
    for (int i = 0; i < suite->num_tests; i++) {
        if (!suite->results[i].compression_correct || !suite->results[i].decompression_correct) {
            failed_tests++;
        }
    }
    
    if (failed_tests > 0) {
        printf("  WARNING: %d tests failed!\n", failed_tests);
        free_regression_suite(suite);
        return 1;
    } else {
        printf("  All tests passed.\n");
    }
    
    printf("\nRegression test complete.\n");
    printf("Use this score to track optimization progress:\n");
    printf("  Current Score: %.1f\n", suite->total_score);
    printf("  Higher scores indicate better performance.\n");
    
    free_regression_suite(suite);
    return 0;
}