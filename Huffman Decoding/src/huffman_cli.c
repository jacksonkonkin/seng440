#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "huffman_compress.h"

void print_usage(const char* program_name) {
    printf("M4-Optimized Huffman Compressor\n");
    printf("Usage: %s [OPTIONS] INPUT_FILE OUTPUT_FILE\n\n", program_name);
    printf("Options:\n");
    printf("  -c, --compress     Compress input file (default)\n");
    printf("  -d, --decompress   Decompress input file\n");
    printf("  -t, --test         Test compressed file integrity\n");
    printf("  -v, --verbose      Enable verbose output\n");
    printf("  -h, --help         Show this help message\n\n");
    printf("Examples:\n");
    printf("  %s -c input.txt compressed.huf    # Compress file\n", program_name);
    printf("  %s -d compressed.huf output.txt   # Decompress file\n", program_name);
    printf("  %s -t compressed.huf              # Test file integrity\n", program_name);
}

void print_version(void) {
    printf("M4-Optimized Huffman Compressor v1.0\n");
    printf("Optimized for Apple Silicon (M1/M2/M3/M4)\n");
    printf("Built with ARM64 optimizations\n");
}

int main(int argc, char* argv[]) {
    int compress_mode = 1;  // 1 = compress, 0 = decompress, -1 = test
    int verbose = 0;
    
    static struct option long_options[] = {
        {"compress",    no_argument, 0, 'c'},
        {"decompress",  no_argument, 0, 'd'},
        {"test",        no_argument, 0, 't'},
        {"verbose",     no_argument, 0, 'v'},
        {"help",        no_argument, 0, 'h'},
        {"version",     no_argument, 0, 'V'},
        {0, 0, 0, 0}
    };
    
    int option_index = 0;
    int c;
    
    while ((c = getopt_long(argc, argv, "cdtvhV", long_options, &option_index)) != -1) {
        switch (c) {
            case 'c':
                compress_mode = 1;
                break;
            case 'd':
                compress_mode = 0;
                break;
            case 't':
                compress_mode = -1;
                break;
            case 'v':
                verbose = 1;
                break;
            case 'h':
                print_usage(argv[0]);
                return 0;
            case 'V':
                print_version();
                return 0;
            case '?':
                print_usage(argv[0]);
                return 1;
            default:
                abort();
        }
    }
    
    // Check arguments
    if (compress_mode == -1) {
        // Test mode - only needs input file
        if (optind >= argc) {
            fprintf(stderr, "Error: Missing input file for test mode\n");
            print_usage(argv[0]);
            return 1;
        }
        
        const char* input_file = argv[optind];
        
        if (verbose) {
            printf("Testing file: %s\n", input_file);
        }
        
        int result = validate_huffman_file(input_file);
        if (result == 0) {
            printf("File validation: PASSED\n");
            return 0;
        } else {
            printf("File validation: FAILED\n");
            return 1;
        }
    } else {
        // Compress/decompress mode - needs input and output files
        if (optind + 1 >= argc) {
            fprintf(stderr, "Error: Missing input and/or output file\n");
            print_usage(argv[0]);
            return 1;
        }
        
        const char* input_file = argv[optind];
        const char* output_file = argv[optind + 1];
        
        if (verbose) {
            printf("Input file:  %s\n", input_file);
            printf("Output file: %s\n", output_file);
            printf("Mode: %s\n", compress_mode ? "Compress" : "Decompress");
        }
        
        int result;
        if (compress_mode) {
            if (verbose) printf("Starting compression...\n");
            result = huffman_compress_file(input_file, output_file);
        } else {
            if (verbose) printf("Starting decompression...\n");
            result = huffman_decompress_file(input_file, output_file);
        }
        
        if (result == 0) {
            if (verbose) printf("Operation completed successfully!\n");
            return 0;
        } else {
            fprintf(stderr, "Error: Operation failed\n");
            return 1;
        }
    }
}