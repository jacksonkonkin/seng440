# M4-Optimized Huffman Decoder

A high-performance Huffman decoder implementation optimized for Apple's M4 ARM architecture, leveraging advanced ARM64 features for maximum throughput.

## Features

- **ARM64 Optimization**: Utilizes M4's specialized instructions (CLZ, RBIT, bit manipulation)
- **NEON SIMD**: Parallel processing of multiple decode streams using 128-bit SIMD
- **Cache-Optimized**: Designed for M4's memory hierarchy with optimal cache usage
- **Multi-Stream**: Supports parallel decoding of 8-16 independent streams
- **High Performance**: Targets 5-10x improvement over naive implementations

## Architecture Support

- **Primary**: Apple M4 (ARMv9.2a)
- **Compatible**: Apple M1/M2/M3 (fallback optimizations)
- **Minimum**: ARM64 with NEON support

## Performance Targets

- **Throughput**: 5-10x improvement over reference implementations
- **Cache Hit Rate**: >95% L1 cache utilization
- **Branch Prediction**: >90% accuracy
- **Memory Bandwidth**: Efficient utilization of 120GB/s available bandwidth

## Quick Start

### Prerequisites

- macOS with Apple Silicon (M1/M2/M3/M4)
- CMake 3.20+
- Apple Clang or GCC with ARM64 support
- Xcode (optional, for debugging and profiling)

### Build

```bash
# Create build directory
mkdir build && cd build

# Configure with CMake
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build
make -j$(nproc)
```

### Usage

```bash
# Basic decoding
./huffman_decoder input.huf output.txt

# Run tests
make test_all

# Run benchmarks
make benchmark_all
```

## Project Structure

```
.
├── CMakeLists.txt          # Build configuration
├── README.md              # This file
├── development-plan.md    # Detailed development plan
├── project-specs.md       # Technical specifications
├── src/
│   ├── core/             # Core Huffman implementation
│   ├── arm64/            # ARM64-specific optimizations
│   ├── simd/             # NEON SIMD implementations
│   └── main.cpp          # Application entry point
├── include/
│   ├── core/             # Core headers
│   ├── optimization/     # Optimization headers
│   ├── arm64/            # ARM64 headers
│   └── simd/             # SIMD headers
├── tests/                # Test suite
├── benchmarks/           # Performance benchmarks
└── docs/                 # Documentation
```

## Development Phases

1. **Phase 1**: Project Setup (Current)
2. **Phase 2**: Core Data Structures
3. **Phase 3**: Basic Decoder Implementation
4. **Phase 4**: ARM64 Optimization
5. **Phase 5**: Cache Optimization
6. **Phase 6**: Multi-stream Processing
7. **Phase 7**: Testing & Benchmarking
8. **Phase 8**: Profiling & Tuning

## Key Optimizations

### ARM64 Instructions
- **CLZ**: Hardware bit counting for efficient length calculations
- **RBIT**: Single-cycle bit reversal operations
- **Shift Operations**: Zero-cost bit manipulation with barrel shifter
- **Conditional Select**: Branch-free conditional operations

### NEON SIMD
- **Parallel Processing**: Simultaneous decode of multiple streams
- **Vectorized Lookups**: Parallel symbol resolution
- **128-bit Operations**: Efficient use of vector registers

### Cache Optimization
- **64-byte Alignment**: Structures aligned to cache line boundaries
- **L1 Cache Targeting**: Decode tables sized for 512KB L1 data cache
- **Prefetch Hints**: Software prefetching for predictable patterns

## Testing

The project includes comprehensive testing:

```bash
# Run all tests
./run_tests

# Run specific test category
./run_tests --category=core
./run_tests --category=arm64
./run_tests --category=simd
```

## Benchmarking

Performance benchmarks with various data patterns:

```bash
# Run all benchmarks
./run_benchmarks

# Run specific benchmark
./run_benchmarks --benchmark=throughput
./run_benchmarks --benchmark=latency
./run_benchmarks --benchmark=cache
```

## Profiling

Use Apple's Instruments for detailed performance analysis:

```bash
# Build with profiling support
cmake .. -DCMAKE_BUILD_TYPE=RelWithDebInfo

# Profile with Instruments
instruments -t "Time Profiler" ./huffman_decoder large_file.huf
```

## Contributing

1. Follow the existing code style and conventions
2. Add tests for new functionality
3. Update benchmarks for performance changes
4. Document ARM64-specific optimizations
5. Test on actual M4 hardware when possible

## License

MIT License - See LICENSE file for details

## Technical References

- [ARM Architecture Reference Manual](https://developer.arm.com/documentation/ddi0487/latest)
- [Apple Silicon Optimization Guide](https://developer.apple.com/documentation/apple-silicon)
- [NEON Programming Guide](https://developer.arm.com/documentation/den0018/latest)

## Status

🚧 **In Development** - Currently in Phase 1 (Project Setup)

See [development-plan.md](development-plan.md) for detailed progress tracking.