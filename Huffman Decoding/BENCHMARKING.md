# M4-Optimized Huffman Compression Benchmarking Guide

## Quick Start

### Basic Benchmark Commands
```bash
# Quick benchmark (5 iterations)
./run_benchmarks.sh quick

# Benchmark specific file
./huffman_benchmark -f sample.txt -v

# Full performance analysis
./run_benchmarks.sh profile
```

## Benchmark Tools

### 1. `huffman_benchmark` - Core Benchmark Executable
```bash
# Options:
-i N          # Number of iterations (default: 10)
-v            # Verbose output with detailed stats  
-a            # Run all synthetic tests (default)
-t TYPE       # Run specific test: text|random|repetitive|binary
-f FILE       # Benchmark specific file
-h            # Help

# Examples:
./huffman_benchmark -i 20 -a           # All tests, 20 iterations
./huffman_benchmark -t text -v         # Text only, verbose
./huffman_benchmark -f large_file.txt  # Specific file
```

### 2. `run_benchmarks.sh` - Convenience Script
```bash
# Usage: ./run_benchmarks.sh [type]
quick     # Quick test (5 iterations)
full      # Full test (20 iterations) 
file FILE # Test specific file
testdata  # Test all generated data files
profile   # Performance profiling
```

## Understanding Results

### Performance Metrics
- **Comp(ms)**: Compression time in milliseconds
- **Decomp(ms)**: Decompression time in milliseconds  
- **Comp MB/s**: Compression throughput (higher = better)
- **Ratio**: Compression ratio (higher = better compression)
- **Space**: Space savings percentage

### Baseline Performance (Current Implementation)
| Data Type | Size | Comp MB/s | Decomp MB/s | Ratio | Notes |
|-----------|------|-----------|-------------|-------|-------|
| Text | 64KB | ~250 | ~75 | 1.8:1 | Natural language |
| Repetitive | 64KB | ~350 | ~175 | 4.0:1 | Best case scenario |
| Random | 64KB | ~180 | ~60 | 1.0:1 | Worst case scenario |
| Source Code | 184KB | ~240 | ~75 | 1.8:1 | Structured text |

### What to Measure for Optimization

1. **Before Optimization**: Run baseline benchmarks
   ```bash
   ./run_benchmarks.sh profile > baseline_results.txt
   ```

2. **After Each Optimization**: Compare improvements
   ```bash
   ./run_benchmarks.sh profile > optimized_results.txt
   # Compare files to see improvements
   ```

3. **Key Metrics to Track**:
   - Compression throughput (MB/s) - should increase
   - Decompression throughput (MB/s) - should increase
   - Memory usage (future enhancement)
   - Compression ratio - should remain stable

## Test Data Types

### Generated Test Files (`test_data/`)
- `large_text.txt` (98KB) - English text patterns
- `repetitive.txt` (1MB) - Highly repetitive data
- `mixed_binary.dat` (2MB) - Random binary data
- `source_code.c` (184KB) - C source code patterns
- `structured_data.json` (248KB) - JSON-like structured data

### Synthetic Data Types
- **Text**: English language patterns, good compression
- **Random**: No patterns, poor compression  
- **Repetitive**: Highly repetitive, excellent compression
- **Binary**: Mixed binary patterns, moderate compression

## ARM64 Optimization Targets

Based on M4 specifications, target improvements:

### Current Performance Bottlenecks
1. **Tree Traversal**: Pointer chasing in decode loop
2. **Bit Stream**: Sequential bit processing  
3. **Memory Access**: Non-optimal cache usage

### Optimization Opportunities
1. **CLZ Instructions**: Use `__builtin_clz()` for bit counting
2. **NEON SIMD**: Parallel symbol processing
3. **Cache Optimization**: 64-byte aligned structures
4. **Branch Prediction**: Reduce unpredictable branches

### Expected Improvements
- **Compression**: 2-3x throughput increase
- **Decompression**: 3-5x throughput increase
- **Memory**: Better cache hit rates (>95%)

## Profiling with Instruments

For detailed profiling on macOS:
```bash
# Build with debug symbols
gcc -I include/core -O3 -g -o huffman_benchmark src/benchmark_runner.c src/core/*.c

# Profile with Instruments
instruments -t "Time Profiler" ./huffman_benchmark -f test_data/large_text.txt

# CPU counters profiling
instruments -t "CPU Counters" ./huffman_benchmark -a
```

## Continuous Performance Testing

### Pre-commit Hook Example
```bash
#!/bin/bash
# Run quick benchmark before each commit
./run_benchmarks.sh quick > /tmp/benchmark_results.txt
echo "Performance check completed - see /tmp/benchmark_results.txt"
```

### Regression Testing
1. Save baseline results: `./run_benchmarks.sh profile > baseline.txt`
2. After changes: `./run_benchmarks.sh profile > current.txt`  
3. Compare: Look for performance regressions

## Memory Profiling (Future Enhancement)

Planned additions:
- Peak memory usage tracking
- Cache hit rate measurements  
- Memory allocation profiling
- SIMD instruction utilization

---

**Next Steps**: Use these benchmarks to measure the effectiveness of your ARM64 SIMD and cache optimizations!