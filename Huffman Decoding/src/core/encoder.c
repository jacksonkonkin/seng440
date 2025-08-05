#include "encoder.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Priority queue for building Huffman tree
typedef struct pq_node {
    encoder_node_t* node;
    struct pq_node* next;
} pq_node_t;

typedef struct priority_queue {
    pq_node_t* head;
    size_t size;
} priority_queue_t;

static priority_queue_t* pq_create(void) {
    priority_queue_t* pq = malloc(sizeof(priority_queue_t));
    if (!pq) return NULL;
    pq->head = NULL;
    pq->size = 0;
    return pq;
}

static void pq_insert(priority_queue_t* pq, encoder_node_t* node) {
    pq_node_t* new_node = malloc(sizeof(pq_node_t));
    if (!new_node) return;
    
    new_node->node = node;
    new_node->next = NULL;
    
    if (!pq->head || node->frequency < pq->head->node->frequency ||
        (node->frequency == pq->head->node->frequency && !node->is_leaf && pq->head->node->is_leaf)) {
        new_node->next = pq->head;
        pq->head = new_node;
    } else {
        pq_node_t* current = pq->head;
        while (current->next && 
               (current->next->node->frequency < node->frequency ||
                (current->next->node->frequency == node->frequency && current->next->node->is_leaf && !node->is_leaf))) {
            current = current->next;
        }
        new_node->next = current->next;
        current->next = new_node;
    }
    pq->size++;
}

static encoder_node_t* pq_extract_min(priority_queue_t* pq) {
    if (!pq->head) return NULL;
    
    pq_node_t* min_node = pq->head;
    encoder_node_t* result = min_node->node;
    pq->head = pq->head->next;
    free(min_node);
    pq->size--;
    return result;
}

static void pq_destroy(priority_queue_t* pq) {
    while (pq->head) {
        pq_node_t* temp = pq->head;
        pq->head = pq->head->next;
        free(temp);
    }
    free(pq);
}

frequency_table_t* frequency_table_create(void) {
    frequency_table_t* table = malloc(sizeof(frequency_table_t));
    if (!table) return NULL;
    
    memset(table->frequencies, 0, sizeof(table->frequencies));
    table->unique_symbols = 0;
    return table;
}

void frequency_table_destroy(frequency_table_t* table) {
    if (table) free(table);
}

int frequency_table_analyze(frequency_table_t* table, const uint8_t* data, size_t length) {
    if (!table || !data) return -1;
    
    memset(table->frequencies, 0, sizeof(table->frequencies));
    table->unique_symbols = 0;
    
    for (size_t i = 0; i < length; i++) {
        if (table->frequencies[data[i]] == 0) {
            table->unique_symbols++;
        }
        table->frequencies[data[i]]++;
    }
    
    return 0;
}

static encoder_node_t* create_encoder_node(uint8_t symbol, uint64_t frequency, bool is_leaf) {
    encoder_node_t* node = malloc(sizeof(encoder_node_t));
    if (!node) return NULL;
    
    node->symbol = symbol;
    node->frequency = frequency;
    node->is_leaf = is_leaf;
    node->left = NULL;
    node->right = NULL;
    return node;
}

encoder_node_t* build_huffman_tree(const frequency_table_t* freq_table) {
    if (!freq_table || freq_table->unique_symbols == 0) return NULL;
    
    priority_queue_t* pq = pq_create();
    if (!pq) return NULL;
    
    // Add all symbols with non-zero frequency to priority queue
    for (int i = 0; i < MAX_SYMBOLS; i++) {
        if (freq_table->frequencies[i] > 0) {
            encoder_node_t* node = create_encoder_node(i, freq_table->frequencies[i], true);
            if (node) {
                pq_insert(pq, node);
            }
        }
    }
    
    // Handle single symbol case
    if (pq->size == 1) {
        encoder_node_t* single = pq_extract_min(pq);
        encoder_node_t* root = create_encoder_node(0, single->frequency, false);
        if (root) {
            root->left = single;
        }
        pq_destroy(pq);
        return root;
    }
    
    // Build tree by combining nodes
    while (pq->size > 1) {
        encoder_node_t* left = pq_extract_min(pq);
        encoder_node_t* right = pq_extract_min(pq);
        
        encoder_node_t* parent = create_encoder_node(0, left->frequency + right->frequency, false);
        if (!parent) {
            pq_destroy(pq);
            return NULL;
        }
        
        parent->left = left;
        parent->right = right;
        pq_insert(pq, parent);
    }
    
    encoder_node_t* root = pq_extract_min(pq);
    pq_destroy(pq);
    return root;
}

void destroy_encoder_tree(encoder_node_t* root) {
    if (!root) return;
    
    destroy_encoder_tree(root->left);
    destroy_encoder_tree(root->right);
    free(root);
}

static void generate_codes_recursive(encoder_node_t* node, code_table_t* table, uint32_t code, uint8_t depth) {
    if (!node || !table) return;
    
    if (node->is_leaf) {
        table->codes[node->symbol].code = code;
        table->codes[node->symbol].length = depth;
        table->codes[node->symbol].valid = true;
        if (depth > table->max_length) {
            table->max_length = depth;
        }
        return;
    }
    
    if (node->left) {
        generate_codes_recursive(node->left, table, code << 1, depth + 1);
    }
    if (node->right) {
        generate_codes_recursive(node->right, table, (code << 1) | 1, depth + 1);
    }
}

code_table_t* generate_codes(encoder_node_t* root) {
    if (!root) return NULL;
    
    code_table_t* table = malloc(sizeof(code_table_t));
    if (!table) return NULL;
    
    memset(table->codes, 0, sizeof(table->codes));
    table->max_length = 0;
    
    // Handle single symbol case
    if (!root->left && !root->right) {
        if (root->is_leaf) {
            table->codes[root->symbol].code = 0;
            table->codes[root->symbol].length = 1;
            table->codes[root->symbol].valid = true;
            table->max_length = 1;
        }
        return table;
    }
    
    generate_codes_recursive(root, table, 0, 0);
    return table;
}

void code_table_destroy(code_table_t* table) {
    if (table) free(table);
}

bit_writer_t* bit_writer_create(void) {
    bit_writer_t* writer = malloc(sizeof(bit_writer_t));
    if (!writer) return NULL;
    
    writer->buffer_capacity = 1024;
    writer->buffer = malloc(writer->buffer_capacity);
    if (!writer->buffer) {
        free(writer);
        return NULL;
    }
    
    writer->buffer_size = 0;
    writer->bit_buffer = 0;
    writer->bits_in_buffer = 0;
    return writer;
}

void bit_writer_destroy(bit_writer_t* writer) {
    if (!writer) return;
    if (writer->buffer) free(writer->buffer);
    free(writer);
}

static int bit_writer_expand(bit_writer_t* writer) {
    size_t new_capacity = writer->buffer_capacity * 2;
    uint8_t* new_buffer = realloc(writer->buffer, new_capacity);
    if (!new_buffer) return -1;
    
    writer->buffer = new_buffer;
    writer->buffer_capacity = new_capacity;
    return 0;
}

int bit_writer_write_bits(bit_writer_t* writer, uint32_t bits, uint8_t count) {
    if (!writer || count > 32) return -1;
    
    writer->bit_buffer = (writer->bit_buffer << count) | bits;
    writer->bits_in_buffer += count;
    
    while (writer->bits_in_buffer >= 8) {
        if (writer->buffer_size >= writer->buffer_capacity) {
            if (bit_writer_expand(writer) != 0) return -1;
        }
        
        uint8_t byte = (writer->bit_buffer >> (writer->bits_in_buffer - 8)) & 0xFF;
        writer->buffer[writer->buffer_size++] = byte;
        writer->bits_in_buffer -= 8;
    }
    
    return 0;
}

int bit_writer_write_code(bit_writer_t* writer, const huffman_code_t* code) {
    if (!writer || !code || !code->valid) return -1;
    return bit_writer_write_bits(writer, code->code, code->length);
}

int bit_writer_flush(bit_writer_t* writer) {
    if (!writer) return -1;
    
    if (writer->bits_in_buffer > 0) {
        if (writer->buffer_size >= writer->buffer_capacity) {
            if (bit_writer_expand(writer) != 0) return -1;
        }
        
        uint8_t byte = (writer->bit_buffer << (8 - writer->bits_in_buffer)) & 0xFF;
        writer->buffer[writer->buffer_size++] = byte;
        writer->bits_in_buffer = 0;
        writer->bit_buffer = 0;
    }
    
    return 0;
}

uint8_t* bit_writer_get_data(bit_writer_t* writer, size_t* size) {
    if (!writer || !size) return NULL;
    *size = writer->buffer_size;
    return writer->buffer;
}