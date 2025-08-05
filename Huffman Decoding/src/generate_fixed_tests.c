#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// Generate exactly reproducible test data for consistent benchmarking
// These patterns are designed to test specific aspects of Huffman compression

void write_file(const char* filename, const uint8_t* data, size_t size) {
    FILE* f = fopen(filename, "wb");
    if (!f) {
        printf("Error: Cannot create %s\n", filename);
        return;
    }
    fwrite(data, 1, size, f);
    fclose(f);
    printf("Generated: %s (%zu bytes)\n", filename, size);
}

void generate_standard_text() {
    // Fixed English text pattern - always generates same data
    const char* base_text = 
        "The quick brown fox jumps over the lazy dog. "
        "This pangram contains every letter of the alphabet. "
        "Pack my box with five dozen liquor jugs. "
        "How vexingly quick daft zebras jump! "
        "Bright vixens jump; dozy fowl quack. "
        "Sphinx of black quartz, judge my vow. ";
    
    size_t base_len = strlen(base_text);
    size_t target_size = 8192; // Exactly 8KB
    
    uint8_t* data = malloc(target_size);
    for (size_t i = 0; i < target_size; i++) {
        data[i] = base_text[i % base_len];
    }
    
    write_file("fixed_tests/standard_text_8k.dat", data, target_size);
    free(data);
}

void generate_high_entropy() {
    // Pseudo-random data with fixed seed for consistency
    uint32_t seed = 0x12345678;
    size_t size = 8192;
    uint8_t* data = malloc(size);
    
    for (size_t i = 0; i < size; i++) {
        seed = seed * 1103515245 + 12345; // Linear congruential generator
        data[i] = (seed >> 16) & 0xFF;
    }
    
    write_file("fixed_tests/high_entropy_8k.dat", data, size);
    free(data);
}

void generate_low_entropy() {
    // Very repetitive pattern - best case for compression
    size_t size = 8192;
    uint8_t* data = malloc(size);
    
    const uint8_t pattern[] = {'A', 'A', 'A', 'B', 'B', 'C'};
    size_t pattern_len = sizeof(pattern);
    
    for (size_t i = 0; i < size; i++) {
        data[i] = pattern[i % pattern_len];
    }
    
    write_file("fixed_tests/low_entropy_8k.dat", data, size);
    free(data);
}

void generate_ascii_code() {
    // C-like source code pattern
    size_t size = 8192;
    uint8_t* data = malloc(size);
    size_t pos = 0;
    
    const char* code_patterns[] = {
        "#include <stdio.h>\n",
        "int main() {\n",
        "    for (int i = 0; i < 100; i++) {\n",
        "        printf(\"Hello World %d\\n\", i);\n",
        "    }\n",
        "    return 0;\n",
        "}\n\n",
        "// This is a comment\n",
        "static void function() {\n",
        "    int variable = 42;\n",
        "    char buffer[256];\n",
        "    memset(buffer, 0, sizeof(buffer));\n",
        "}\n\n"
    };
    
    int num_patterns = sizeof(code_patterns) / sizeof(code_patterns[0]);
    
    while (pos < size) {
        for (int i = 0; i < num_patterns && pos < size; i++) {
            const char* pattern = code_patterns[i];
            size_t pattern_len = strlen(pattern);
            
            for (size_t j = 0; j < pattern_len && pos < size; j++) {
                data[pos++] = pattern[j];
            }
        }
    }
    
    write_file("fixed_tests/ascii_code_8k.dat", data, size);
    free(data);
}

void generate_binary_mixed() {
    // Mixed binary data with specific patterns
    size_t size = 8192;
    uint8_t* data = malloc(size);
    
    for (size_t i = 0; i < size; i++) {
        if (i % 16 == 0) data[i] = 0x00;
        else if (i % 16 == 15) data[i] = 0xFF;
        else if (i % 4 == 0) data[i] = 0xAA;
        else if (i % 4 == 1) data[i] = 0x55;
        else data[i] = (i * 17 + 42) & 0xFF;
    }
    
    write_file("fixed_tests/binary_mixed_8k.dat", data, size);
    free(data);
}

void generate_large_text() {
    // Larger text file for testing scalability
    const char* base_text = 
        "In the beginning was the Word, and the Word was with God, and the Word was God. "
        "All things were made through him, and without him was not any thing made that was made. "
        "The quick brown fox jumps over the lazy dog, demonstrating all letters of the alphabet. "
        "Programming languages like C provide low-level control over system resources. "
        "Huffman coding is a lossless data compression algorithm that uses variable-length codes. "
        "The Apple M4 chip features ARM64 architecture with advanced SIMD capabilities. ";
    
    size_t base_len = strlen(base_text);
    size_t target_size = 65536; // Exactly 64KB
    
    uint8_t* data = malloc(target_size);
    for (size_t i = 0; i < target_size; i++) {
        data[i] = base_text[i % base_len];
    }
    
    write_file("fixed_tests/large_text_64k.dat", data, target_size);
    free(data);
}

void generate_frequency_test() {
    // Specific frequency distribution for testing Huffman tree building
    size_t size = 4096;
    uint8_t* data = malloc(size);
    size_t pos = 0;
    
    // Character frequencies designed to create specific tree shapes
    // 'A': 50% frequency
    for (int i = 0; i < size/2 && pos < size; i++) data[pos++] = 'A';
    
    // 'B': 25% frequency  
    for (int i = 0; i < size/4 && pos < size; i++) data[pos++] = 'B';
    
    // 'C': 12.5% frequency
    for (int i = 0; i < size/8 && pos < size; i++) data[pos++] = 'C';
    
    // 'D': 6.25% frequency
    for (int i = 0; i < size/16 && pos < size; i++) data[pos++] = 'D';
    
    // Fill remaining with 'E'
    while (pos < size) data[pos++] = 'E';
    
    write_file("fixed_tests/frequency_test_4k.dat", data, size);
    free(data);
}

int main() {
    printf("Generating fixed test data for consistent benchmarking...\n");
    
    // Create directory
    system("mkdir -p fixed_tests");
    
    // Generate all test files
    generate_standard_text();      // Typical English text
    generate_high_entropy();       // Hard to compress (random-like)  
    generate_low_entropy();        // Easy to compress (repetitive)
    generate_ascii_code();         // Source code patterns
    generate_binary_mixed();       // Binary data with patterns
    generate_large_text();         // Larger dataset (64KB)
    generate_frequency_test();     // Specific frequency distribution
    
    printf("\nFixed test data generation complete!\n");
    printf("All files use deterministic generation - same data every time.\n");
    printf("Use these files for consistent performance measurements.\n");
    
    return 0;
}