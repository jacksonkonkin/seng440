# M4-Optimized Huffman Compressor

A high-performance Huffman compressor optimized for Apple's M4 ARM architecture, with comprehensive benchmarking and regression testing for optimization tracking.

## 🚀 Quick Start

### Build
```bash
# Build all tools
gcc -I include/core -O3 -o huffman src/huffman_cli.c src/core/*.c
gcc -I include/core -O3 -o huffman_benchmark src/benchmark_runner.c src/core/*.c  
gcc -I include/core -O3 -o regression_test src/regression_runner.c src/core/*.c
gcc -I include/core -O3 -o generate_fixed_tests src/generate_fixed_tests.c

# Or use CMake
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make
```

### Usage
```bash
# Compress file
./huffman -c input.txt compressed.huf

# Decompress file  
./huffman -d compressed.huf output.txt

# Benchmark performance
./huffman_benchmark -f sample.txt -v

# Track optimization progress
./track_optimization.sh baseline          # Establish baseline
./track_optimization.sh test "my-opt"     # Test optimization
```

## 📁 Repository Structure

```
├── src/
│   ├── core/                  # Core compression implementation
│   │   ├── huffman_tree.c     # Huffman tree construction
│   │   ├── encoder.c          # Compression engine
│   │   ├── decoder.c          # Decompression engine
│   │   ├── bit_stream.c       # Bit-level I/O
│   │   ├── file_format.c      # File format handling
│   │   ├── huffman_compress.c # High-level API
│   │   ├── benchmark.c        # Performance measurement
│   │   └── regression_test.c  # Fixed test suite
│   ├── huffman_cli.c          # Command-line interface
│   ├── benchmark_runner.c     # Benchmark tool
│   ├── regression_runner.c    # Regression test tool
│   └── generate_fixed_tests.c # Test data generator
├── include/core/              # Header files
├── fixed_tests/               # Fixed test data (generated)
├── sample.txt                 # Sample file for testing
├── track_optimization.sh      # Optimization tracking script
├── BENCHMARKING.md           # Benchmarking guide
└── REGRESSION_TESTING.md     # Regression testing guide
```

## 🎯 ARM64 Optimization Ready

The codebase is structured for easy ARM64 optimization:

### Current Performance (Baseline)
- **Text Compression**: ~230 MB/s
- **Text Decompression**: ~70 MB/s
- **Compression Ratio**: 1.7:1 (typical text)

### Optimization Targets
- **CLZ Instructions**: Use `__builtin_clz()` for bit counting
- **NEON SIMD**: Parallel symbol processing
- **Cache Optimization**: 64-byte aligned structures
- **Expected Improvement**: 2-5x throughput increase

### Key Files to Optimize
- `src/core/decoder.c:41-66` - Main decode loop
- `src/core/bit_stream.c:33-46` - Bit reading operations
- `src/core/huffman_tree.c` - Tree traversal

## 🔧 Tools Included

### 1. `huffman` - Main Compressor
```bash
./huffman [OPTIONS] INPUT_FILE OUTPUT_FILE
  -c, --compress     Compress file (default)
  -d, --decompress   Decompress file  
  -t, --test         Test file integrity
  -v, --verbose      Verbose output
```

### 2. `huffman_benchmark` - Performance Testing
```bash
./huffman_benchmark [OPTIONS]
  -f FILE           Benchmark specific file
  -i N              Number of iterations
  -v                Verbose output
  -a                Run all synthetic tests
```

### 3. `regression_test` - Fixed Test Suite
```bash
./regression_test [OPTIONS]
  -v VERSION        Version identifier
  -i N              Iterations per test  
  -o FILE           Save results to JSON
  -s                Summary only
```

### 4. `track_optimization.sh` - Optimization Tracker
```bash
./track_optimization.sh baseline     # Establish baseline
./track_optimization.sh test "name"  # Test optimization
./track_optimization.sh best         # Show best results
./track_optimization.sh list         # List all results
```

## 📊 Performance Measurement

### Benchmarking Workflow
1. **Establish Baseline**: `./track_optimization.sh baseline`
2. **Make Optimizations**: Edit source code
3. **Test Performance**: `./track_optimization.sh test "optimization-name"`  
4. **Track Progress**: Automatic comparison with baseline

### Fixed Test Suite
7 standardized tests that never change:
- **StandardText8K**: Real-world text performance
- **HighEntropy8K**: Worst-case (random data)
- **LowEntropy8K**: Best-case (repetitive data)  
- **AsciiCode8K**: Source code patterns
- **BinaryMixed8K**: Binary data patterns
- **LargeText64K**: Scalability test
- **FrequencyTest4K**: Tree building test

## 🎯 Optimization Workflow

### Step-by-Step Process
```bash
# 1. Establish baseline
./track_optimization.sh baseline
# Result: Score 132.0

# 2. Add CLZ optimization to decoder.c
# Replace manual bit counting with __builtin_clz()

# 3. Test improvement  
./track_optimization.sh test "clz-v1"
# Result: Score 185.3 (+40% improvement!)

# 4. Add NEON SIMD optimization
# Implement parallel symbol processing

# 5. Test improvement
./track_optimization.sh test "clz-neon-v1"  
# Result: Score 267.8 (+103% improvement!)

# 6. Continue optimizing...
./track_optimization.sh test "clz-neon-cache-v1"
```

## 📖 Documentation

- **[BENCHMARKING.md](BENCHMARKING.md)** - Complete benchmarking guide
- **[REGRESSION_TESTING.md](REGRESSION_TESTING.md)** - Regression testing system
- **[project-specs.md](project-specs.md)** - M4 architecture details

## 🔧 ARM64 Development

### Compiler Flags (Already Configured)
```bash
-arch arm64 -mcpu=apple-m1 -O3
-ffast-math -funroll-loops -fvectorize
-falign-functions=64 -falign-loops=64
```

### Key ARM64 Features to Utilize
- **CLZ**: Count Leading Zeros instruction
- **NEON**: 128-bit SIMD processing
- **Barrel Shifter**: Zero-cost bit manipulation
- **Conditional Select**: Branch-free operations

### Expected Performance Improvements
| Optimization | Throughput Gain | Implementation |
|--------------|----------------|----------------|
| CLZ Instructions | +30-50% | Replace loops with `__builtin_clz()` |
| NEON SIMD | +100-200% | Parallel symbol processing |
| Cache Alignment | +20-40% | 64-byte aligned structures |
| **Combined** | **+200-400%** | All optimizations together |

## 🧪 Testing

```bash
# Quick functionality test
./huffman -c sample.txt test.huf
./huffman -d test.huf restored.txt  
diff sample.txt restored.txt

# Performance benchmark
./huffman_benchmark -f sample.txt -v

# Regression test
./regression_test -v "current" -i 20 -s
```

## 📈 Current Status

- ✅ **Core Implementation**: Complete Huffman compression/decompression
- ✅ **File Format**: Custom .huf format with integrity checking
- ✅ **Benchmarking**: Comprehensive performance measurement
- ✅ **Regression Testing**: Fixed test suite for optimization tracking
- 🔄 **ARM64 Optimization**: Ready for implementation

---

**Ready for ARM64 optimization work!** Use the benchmarking system to measure exactly how much faster your optimizations make the code.