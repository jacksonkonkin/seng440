#include "huffman_compress.h"
#include <stdlib.h>
#include <string.h>

huffman_context_t* huffman_context_create(void) {
    huffman_context_t* ctx = malloc(sizeof(huffman_context_t));
    if (!ctx) return NULL;
    
    ctx->freq_table = NULL;
    ctx->code_table = NULL;
    ctx->tree_root = NULL;
    ctx->decode_tree = NULL;
    ctx->writer = NULL;
    
    return ctx;
}

void huffman_context_destroy(huffman_context_t* ctx) {
    if (!ctx) return;
    
    if (ctx->freq_table) frequency_table_destroy(ctx->freq_table);
    if (ctx->code_table) code_table_destroy(ctx->code_table);
    if (ctx->tree_root) destroy_encoder_tree(ctx->tree_root);
    if (ctx->decode_tree) huffman_tree_destroy(ctx->decode_tree);
    if (ctx->writer) bit_writer_destroy(ctx->writer);
    
    free(ctx);
}

static uint8_t* read_file_data(const char* path, size_t* size) {
    FILE* file = fopen(path, "rb");
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

static int write_file_data(const char* path, const uint8_t* data, size_t size) {
    FILE* file = fopen(path, "wb");
    if (!file) return -1;
    
    size_t written = fwrite(data, 1, size, file);
    fclose(file);
    
    return (written == size) ? 0 : -1;
}

int huffman_compress_data(const uint8_t* data, size_t data_size, 
                         uint8_t** compressed_data, size_t* compressed_size,
                         symbol_info_t** symbol_table, size_t* symbol_count) {
    if (!data || !compressed_data || !compressed_size || !symbol_table || !symbol_count) {
        return -1;
    }
    
    huffman_context_t* ctx = huffman_context_create();
    if (!ctx) return -1;
    
    // Analyze frequencies
    ctx->freq_table = frequency_table_create();
    if (!ctx->freq_table || frequency_table_analyze(ctx->freq_table, data, data_size) != 0) {
        huffman_context_destroy(ctx);
        return -1;
    }
    
    // Build tree and generate codes
    ctx->tree_root = build_huffman_tree(ctx->freq_table);
    if (!ctx->tree_root) {
        huffman_context_destroy(ctx);
        return -1;
    }
    
    ctx->code_table = generate_codes(ctx->tree_root);
    if (!ctx->code_table) {
        huffman_context_destroy(ctx);
        return -1;
    }
    
    // Create symbol table for output
    *symbol_count = ctx->freq_table->unique_symbols;
    *symbol_table = malloc(sizeof(symbol_info_t) * (*symbol_count));
    if (!*symbol_table) {
        huffman_context_destroy(ctx);
        return -1;
    }
    
    size_t table_index = 0;
    for (int i = 0; i < MAX_SYMBOLS; i++) {
        if (ctx->code_table->codes[i].valid) {
            (*symbol_table)[table_index].symbol = i;
            (*symbol_table)[table_index].code_length = ctx->code_table->codes[i].length;
            (*symbol_table)[table_index].code = ctx->code_table->codes[i].code;
            table_index++;
        }
    }
    
    // Encode data
    ctx->writer = bit_writer_create();
    if (!ctx->writer) {
        free(*symbol_table);
        huffman_context_destroy(ctx);
        return -1;
    }
    
    for (size_t i = 0; i < data_size; i++) {
        if (bit_writer_write_code(ctx->writer, &ctx->code_table->codes[data[i]]) != 0) {
            free(*symbol_table);
            huffman_context_destroy(ctx);
            return -1;
        }
    }
    
    if (bit_writer_flush(ctx->writer) != 0) {
        free(*symbol_table);
        huffman_context_destroy(ctx);
        return -1;
    }
    
    // Get compressed data
    uint8_t* writer_data = bit_writer_get_data(ctx->writer, compressed_size);
    *compressed_data = malloc(*compressed_size);
    if (!*compressed_data) {
        free(*symbol_table);
        huffman_context_destroy(ctx);
        return -1;
    }
    
    memcpy(*compressed_data, writer_data, *compressed_size);
    huffman_context_destroy(ctx);
    return 0;
}

int huffman_compress_file(const char* input_path, const char* output_path) {
    if (!input_path || !output_path) return -1;
    
    // Read input file
    size_t data_size;
    uint8_t* data = read_file_data(input_path, &data_size);
    if (!data) return -1;
    
    // Compress data
    uint8_t* compressed_data;
    size_t compressed_size;
    symbol_info_t* symbol_table;
    size_t symbol_count;
    
    int result = huffman_compress_data(data, data_size, &compressed_data, &compressed_size,
                                      &symbol_table, &symbol_count);
    if (result != 0) {
        free(data);
        return -1;
    }
    
    // Create header
    huffman_header_t header = {0};
    header.magic = HUFFMAN_MAGIC;
    header.version = HUFFMAN_VERSION;
    header.flags = 0;
    header.original_size = data_size;
    header.compressed_size = compressed_size;
    header.symbol_count = symbol_count;
    header.max_code_length = 0;
    header.checksum = calculate_crc32(data, data_size);
    
    // Find max code length
    for (size_t i = 0; i < symbol_count; i++) {
        if (symbol_table[i].code_length > header.max_code_length) {
            header.max_code_length = symbol_table[i].code_length;
        }
    }
    
    // Write output file
    FILE* output_file = fopen(output_path, "wb");
    if (!output_file) {
        free(data);
        free(compressed_data);
        free(symbol_table);
        return -1;
    }
    
    // Write header, symbol table, and compressed data
    if (huffman_write_header(output_file, &header) != 0 ||
        huffman_write_symbol_table(output_file, symbol_table, symbol_count) != 0 ||
        fwrite(compressed_data, 1, compressed_size, output_file) != compressed_size) {
        fclose(output_file);
        free(data);
        free(compressed_data);
        free(symbol_table);
        return -1;
    }
    
    fclose(output_file);
    free(data);
    free(compressed_data);
    free(symbol_table);
    
    printf("Compression completed successfully!\n");
    print_compression_stats(data_size, sizeof(huffman_header_t) + 
                           sizeof(symbol_info_t) * symbol_count + compressed_size);
    
    return 0;
}

int huffman_decompress_file(const char* input_path, const char* output_path) {
    if (!input_path || !output_path) return -1;
    
    FILE* input_file = fopen(input_path, "rb");
    if (!input_file) return -1;
    
    // Read header
    huffman_header_t header;
    if (huffman_read_header(input_file, &header) != 0) {
        fclose(input_file);
        return -1;
    }
    
    // Read symbol table
    symbol_info_t* symbol_table = malloc(sizeof(symbol_info_t) * header.symbol_count);
    if (!symbol_table || huffman_read_symbol_table(input_file, symbol_table, header.symbol_count) != 0) {
        fclose(input_file);
        if (symbol_table) free(symbol_table);
        return -1;
    }
    
    // Read compressed data
    uint8_t* compressed_data = malloc(header.compressed_size);
    if (!compressed_data || fread(compressed_data, 1, header.compressed_size, input_file) != header.compressed_size) {
        fclose(input_file);
        free(symbol_table);
        if (compressed_data) free(compressed_data);
        return -1;
    }
    
    fclose(input_file);
    
    // Decompress data
    uint8_t* output_data;
    size_t output_size;
    
    int result = huffman_decompress_data(compressed_data, header.compressed_size,
                                        symbol_table, header.symbol_count,
                                        &output_data, &output_size, header.original_size);
    
    free(compressed_data);
    free(symbol_table);
    
    if (result != 0) return -1;
    
    // Verify size and checksum
    if (output_size != header.original_size) {
        free(output_data);
        return -1;
    }
    
    uint32_t checksum = calculate_crc32(output_data, output_size);
    if (checksum != header.checksum) {
        free(output_data);
        return -1;
    }
    
    // Write output file
    result = write_file_data(output_path, output_data, output_size);
    free(output_data);
    
    if (result == 0) {
        printf("Decompression completed successfully!\n");
    }
    
    return result;
}

int huffman_decompress_data(const uint8_t* compressed_data, size_t compressed_size,
                           const symbol_info_t* symbol_table, size_t symbol_count,
                           uint8_t** output_data, size_t* output_size, size_t expected_size) {
    if (!compressed_data || !symbol_table || !output_data || !output_size) {
        return -1;
    }
    
    // Convert symbol table to arrays for tree building
    uint8_t* symbols = malloc(symbol_count);
    uint8_t* code_lengths = malloc(symbol_count);
    uint32_t* codes = malloc(symbol_count * sizeof(uint32_t));
    
    if (!symbols || !code_lengths || !codes) {
        if (symbols) free(symbols);
        if (code_lengths) free(code_lengths);
        if (codes) free(codes);
        return -1;
    }
    
    for (size_t i = 0; i < symbol_count; i++) {
        symbols[i] = symbol_table[i].symbol;
        code_lengths[i] = symbol_table[i].code_length;
        codes[i] = symbol_table[i].code;
    }
    
    // Build decode tree using actual codes
    huffman_tree_t* tree = huffman_tree_from_code_table(symbols, codes, code_lengths, symbol_count);
    free(symbols);
    free(code_lengths);
    free(codes);
    
    if (!tree) return -1;
    
    // Create bit stream and decoder
    bit_stream_t* stream = bit_stream_create((uint8_t*)compressed_data, compressed_size);
    huffman_decoder_t* decoder = huffman_decoder_create(tree);
    
    if (!stream || !decoder) {
        if (stream) bit_stream_destroy(stream);
        if (decoder) huffman_decoder_destroy(decoder);
        huffman_tree_destroy(tree);
        return -1;
    }
    
    // Allocate output buffer for exact expected size
    uint8_t* temp_output = malloc(expected_size);
    if (!temp_output) {
        bit_stream_destroy(stream);
        huffman_decoder_destroy(decoder);
        huffman_tree_destroy(tree);
        return -1;
    }
    
    // Decode exactly the expected number of bytes
    size_t temp_size = 0;
    while (temp_size < expected_size && bit_stream_has_data(stream)) {
        uint8_t symbol;
        if (huffman_decode_symbol(tree, stream, &symbol) != 0) {
            break;  // End of valid data or error
        }
        
        temp_output[temp_size++] = symbol;
    }
    
    // Cleanup
    bit_stream_destroy(stream);
    huffman_decoder_destroy(decoder);
    huffman_tree_destroy(tree);
    
    // Check if we decoded the expected amount
    if (temp_size != expected_size) {
        free(temp_output);
        return -1;
    }
    
    // Return results
    *output_data = temp_output;
    *output_size = temp_size;
    
    return 0;
}

void print_compression_stats(size_t original_size, size_t compressed_size) {
    if (original_size == 0) {
        printf("Original size: 0 bytes\n");
        return;
    }
    
    double ratio = (double)compressed_size / original_size;
    double savings = (1.0 - ratio) * 100.0;
    
    printf("Original size:   %zu bytes\n", original_size);
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("Compression ratio: %.2f:1\n", 1.0 / ratio);
    printf("Space savings: %.1f%%\n", savings);
}

int validate_huffman_file(const char* path) {
    FILE* file = fopen(path, "rb");
    if (!file) return -1;
    
    huffman_header_t header;
    int result = huffman_read_header(file, &header);
    fclose(file);
    
    return result;
}