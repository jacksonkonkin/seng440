#!/bin/bash

# Optimization Tracking Script for M4 Huffman Compressor
# This script helps track performance improvements during optimization

RESULTS_DIR="optimization_results"
BASELINE_FILE="$RESULTS_DIR/baseline.json"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

print_header() {
    echo -e "${BLUE}=================================================${NC}"
    echo -e "${BLUE}M4 Huffman Optimization Tracker${NC}"
    echo -e "${BLUE}=================================================${NC}"
    echo ""
}

print_usage() {
    echo "Usage: $0 [command] [options]"
    echo ""
    echo "Commands:"
    echo "  baseline [name]     - Establish baseline performance"
    echo "  test [name]         - Run performance test with identifier"
    echo "  compare [file]      - Compare current with saved results"
    echo "  list               - List all saved results"
    echo "  best               - Show best performance achieved"
    echo "  clean              - Clean old result files"
    echo "  help               - Show this help"
    echo ""
    echo "Options:"
    echo "  -i N               - Number of iterations (default: 30)"
    echo "  -v                 - Verbose output"
    echo ""
    echo "Examples:"
    echo "  $0 baseline                    # Establish baseline"
    echo "  $0 test \"clz-optimization\"     # Test CLZ optimization"
    echo "  $0 test \"neon-simd\" -i 50      # Test NEON with 50 iterations"
    echo "  $0 compare baseline.json       # Compare with baseline"
    echo "  $0 best                        # Show best results so far"
}

ensure_regression_tool() {
    if [ ! -f "./regression_test" ]; then
        echo "Building regression test tool..."
        gcc -I include/core -O3 -o regression_test src/regression_runner.c src/core/*.c
        if [ $? -ne 0 ]; then
            echo -e "${RED}Error: Failed to build regression test tool${NC}"
            exit 1
        fi
    fi
}

ensure_results_dir() {
    mkdir -p "$RESULTS_DIR"
}

run_baseline() {
    local name="${1:-baseline}"
    local iterations="${2:-30}"
    
    print_header
    echo -e "${YELLOW}Establishing baseline performance...${NC}"
    echo "Name: $name"
    echo "Iterations: $iterations"
    echo ""
    
    ensure_regression_tool
    ensure_results_dir
    
    local output_file="$RESULTS_DIR/${name}_$(date +%Y%m%d_%H%M%S).json"
    
    ./regression_test -v "$name" -i "$iterations" -o "$output_file"
    
    if [ $? -eq 0 ]; then
        # Copy as baseline if this is the first baseline
        if [ ! -f "$BASELINE_FILE" ]; then
            cp "$output_file" "$BASELINE_FILE"
            echo -e "${GREEN}Baseline established: $BASELINE_FILE${NC}"
        fi
        
        echo -e "${GREEN}Results saved: $output_file${NC}"
        echo ""
        echo "Next steps:"
        echo "1. Make your ARM64 optimizations"
        echo "2. Run: $0 test \"your-optimization-name\""
        echo "3. Compare results with: $0 compare $output_file"
    else
        echo -e "${RED}Baseline test failed!${NC}"
        exit 1
    fi
}

run_test() {
    local name="${1:-current}"
    local iterations="${2:-30}"
    
    print_header
    echo -e "${YELLOW}Running optimization test...${NC}"
    echo "Name: $name"
    echo "Iterations: $iterations"
    echo ""
    
    ensure_regression_tool
    ensure_results_dir
    
    local output_file="$RESULTS_DIR/${name}_$(date +%Y%m%d_%H%M%S).json"
    
    ./regression_test -v "$name" -i "$iterations" -o "$output_file"
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}Test completed: $output_file${NC}"
        
        # Auto-compare with baseline if it exists
        if [ -f "$BASELINE_FILE" ]; then
            echo ""
            echo -e "${BLUE}Comparing with baseline...${NC}"
            compare_results "$BASELINE_FILE" "$output_file"
        fi
    else
        echo -e "${RED}Test failed!${NC}"
        exit 1
    fi
}

extract_score() {
    local file="$1"
    if [ -f "$file" ]; then
        grep '"total_score"' "$file" | cut -d: -f2 | cut -d, -f1 | tr -d ' '
    else
        echo "0"
    fi
}

extract_version() {
    local file="$1"
    if [ -f "$file" ]; then
        grep '"version"' "$file" | cut -d: -f2 | cut -d, -f1 | tr -d ' "' 
    else
        echo "unknown"
    fi
}

compare_results() {
    local baseline_file="$1"
    local current_file="$2"
    
    if [ ! -f "$baseline_file" ]; then
        echo -e "${RED}Baseline file not found: $baseline_file${NC}"
        return 1
    fi
    
    if [ ! -f "$current_file" ]; then
        echo -e "${RED}Current file not found: $current_file${NC}"
        return 1
    fi
    
    local baseline_score=$(extract_score "$baseline_file")
    local current_score=$(extract_score "$current_file")
    local baseline_version=$(extract_version "$baseline_file")
    local current_version=$(extract_version "$current_file")
    
    echo "Performance Comparison:"
    echo "======================"
    echo "Baseline ($baseline_version): $baseline_score"
    echo "Current  ($current_version): $current_score"
    echo ""
    
    if [ -n "$baseline_score" ] && [ -n "$current_score" ]; then
        local improvement=$(echo "scale=2; $current_score - $baseline_score" | bc 2>/dev/null || echo "0")
        local percentage=$(echo "scale=1; ($current_score - $baseline_score) * 100 / $baseline_score" | bc 2>/dev/null || echo "0")
        
        if (( $(echo "$improvement > 0" | bc -l 2>/dev/null || echo "0") )); then
            echo -e "${GREEN}✓ IMPROVEMENT: +$improvement points (+$percentage%)${NC}"
        elif (( $(echo "$improvement < 0" | bc -l 2>/dev/null || echo "0") )); then
            echo -e "${RED}✗ REGRESSION: $improvement points ($percentage%)${NC}"
        else
            echo -e "${YELLOW}= NO CHANGE${NC}"
        fi
    else
        echo -e "${YELLOW}Unable to calculate improvement${NC}"
    fi
    
    echo ""
    echo "Detailed comparison:"
    echo "Run: python3 -c \"
import json
import sys

try:
    with open('$baseline_file', 'r') as f: baseline = json.load(f)
    with open('$current_file', 'r') as f: current = json.load(f)
    
    print('%-15s %10s %10s %10s' % ('Test', 'Baseline', 'Current', 'Change'))
    print('-' * 50)
    
    for i, result in enumerate(current['results']):
        name = result['name']
        curr_comp = result['compress_throughput_mbps']
        curr_decomp = result['decompress_throughput_mbps']
        
        if i < len(baseline['results']):
            base_comp = baseline['results'][i]['compress_throughput_mbps']
            base_decomp = baseline['results'][i]['decompress_throughput_mbps']
            
            comp_change = ((curr_comp - base_comp) / base_comp * 100) if base_comp > 0 else 0
            decomp_change = ((curr_decomp - base_decomp) / base_decomp * 100) if base_decomp > 0 else 0
            
            print('%-15s %9.1f %9.1f %+8.1f%%' % (name[:15], base_comp, curr_comp, comp_change))
            
except Exception as e:
    print('Error comparing files:', e)
\""
}

list_results() {
    ensure_results_dir
    
    echo "Saved Results:"
    echo "=============="
    
    if [ ! -d "$RESULTS_DIR" ] || [ -z "$(ls -A "$RESULTS_DIR" 2>/dev/null)" ]; then
        echo "No results found. Run '$0 baseline' first."
        return
    fi
    
    echo "%-30s %-15s %-10s %-20s" "File" "Version" "Score" "Date"
    echo "$(printf '%.0s-' {1..80})"
    
    for file in "$RESULTS_DIR"/*.json; do
        if [ -f "$file" ]; then
            local filename=$(basename "$file")
            local version=$(extract_version "$file")
            local score=$(extract_score "$file")
            local date=$(stat -f "%Sm" -t "%Y-%m-%d %H:%M" "$file" 2>/dev/null || date -r "$file" "+%Y-%m-%d %H:%M" 2>/dev/null || echo "unknown")
            
            printf "%-30s %-15s %-10s %-20s\n" "${filename:0:29}" "${version:0:14}" "$score" "$date"
        fi
    done
}

show_best() {
    ensure_results_dir
    
    echo "Best Performance Results:"
    echo "========================"
    
    local best_score=0
    local best_file=""
    
    for file in "$RESULTS_DIR"/*.json; do
        if [ -f "$file" ]; then
            local score=$(extract_score "$file")
            if (( $(echo "$score > $best_score" | bc -l 2>/dev/null || echo "0") )); then
                best_score=$score
                best_file=$file
            fi
        fi
    done
    
    if [ -n "$best_file" ]; then
        local version=$(extract_version "$best_file")
        echo "Best Score: $best_score"
        echo "Version: $version"
        echo "File: $best_file"
        echo ""
        echo "To reproduce this performance:"
        echo "1. Restore code to the '$version' state"
        echo "2. Run: $0 test \"reproduce-$version\""
    else
        echo "No results found. Run '$0 baseline' first."
    fi
}

clean_results() {
    echo "Cleaning old result files..."
    
    if [ -d "$RESULTS_DIR" ]; then
        # Keep only the 10 most recent files
        ls -t "$RESULTS_DIR"/*.json 2>/dev/null | tail -n +11 | xargs rm -f 2>/dev/null
        echo "Cleaned old results (kept 10 most recent)"
    else
        echo "No results directory found"
    fi
}

# Parse command line arguments
ITERATIONS=30
VERBOSE=0

while [[ $# -gt 0 ]]; do
    case $1 in
        -i)
            ITERATIONS="$2"
            shift 2
            ;;
        -v)
            VERBOSE=1
            shift
            ;;
        baseline)
            run_baseline "${2:-baseline}" "$ITERATIONS"
            exit 0
            ;;
        test)
            if [ -z "$2" ]; then
                echo "Error: test command requires a name"
                print_usage
                exit 1
            fi
            run_test "$2" "$ITERATIONS"
            exit 0
            ;;
        compare)
            if [ -z "$2" ]; then
                echo "Error: compare command requires a file"
                print_usage
                exit 1
            fi
            compare_results "$2" "${3:-current}"
            exit 0
            ;;
        list)
            list_results
            exit 0
            ;;
        best)
            show_best
            exit 0
            ;;
        clean)
            clean_results
            exit 0
            ;;
        help|--help|-h)
            print_usage
            exit 0
            ;;
        *)
            echo "Unknown command: $1"
            print_usage
            exit 1
            ;;
    esac
done

# Default action if no command specified
print_usage