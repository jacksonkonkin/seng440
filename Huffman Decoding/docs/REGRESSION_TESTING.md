# M4 Huffman Compression Regression Testing System

## 🎯 Overview

This regression testing system provides **fixed, reproducible benchmarks** that stay exactly the same across all optimization iterations. Use these tests to precisely measure performance improvements as you optimize the code.

## 🚀 Quick Start

### 1. Establish Baseline (Before Any Optimization)
```bash
# This creates your baseline performance measurements
./track_optimization.sh baseline
```

**Result**: Baseline score established (e.g., 132.0)

### 2. Make ARM64 Optimizations
- Add CLZ (Count Leading Zeros) instructions
- Implement NEON SIMD parallel processing  
- Optimize cache alignment and memory access patterns

### 3. Test Your Optimizations
```bash
# Test your improvements
./track_optimization.sh test "clz-optimization"
./track_optimization.sh test "neon-simd-v1"
./track_optimization.sh test "cache-aligned"
```

**Result**: Automatic comparison with baseline showing improvement percentage

### 4. Track Best Performance
```bash
# See your best results so far
./track_optimization.sh best

# List all test results
./track_optimization.sh list
```

## 📊 What Gets Measured

### Fixed Test Suite (7 Tests)
Every test uses **exactly the same data** every time:

| Test | Size | Data Type | Expected Ratio | Purpose |
|------|------|-----------|---------------|---------|
| **StandardText8K** | 8KB | English text | 1.7:1 | Real-world text performance |
| **HighEntropy8K** | 8KB | Pseudo-random | 1.0:1 | Worst-case scenario |
| **LowEntropy8K** | 8KB | Repetitive | 5.3:1 | Best-case scenario |
| **AsciiCode8K** | 8KB | Source code | 1.7:1 | Structured text patterns |
| **BinaryMixed8K** | 8KB | Mixed binary | 1.6:1 | Binary data patterns |
| **LargeText64K** | 64KB | Large text | 1.8:1 | Scalability test |
| **FrequencyTest4K** | 4KB | Specific frequencies | 4.3:1 | Tree building test |

### Key Metrics Tracked
- **Compression Throughput** (MB/s) - How fast compression runs
- **Decompression Throughput** (MB/s) - How fast decompression runs  
- **Compression Ratio** - How well it compresses (should stay same)
- **Overall Score** - Weighted performance score for easy comparison

## 🔧 Tools Available

### 1. `regression_test` - Core Test Runner
```bash
# Basic usage
./regression_test -v "test-name" -i 30

# Options:
-v VERSION    # Version identifier
-i N          # Iterations (default: 20)
-o FILE       # Save results to JSON file
-s            # Summary only (less verbose)
-g            # Generate test files if missing
```

### 2. `track_optimization.sh` - Optimization Tracker  
```bash
# Commands:
baseline [name]     # Establish baseline
test [name]         # Test optimization
compare [file]      # Compare with saved results
list               # List all results
best               # Show best performance
clean              # Clean old files
```

### 3. `generate_fixed_tests` - Test Data Generator
```bash
# Regenerate test data (only if needed)
./generate_fixed_tests
```

## 📈 Understanding Results

### Sample Output
```
Test                Size   C(ms)   D(ms)    C(MB/s)    D(MB/s)  Ratio  Score
StandardText8K      8.0K   0.03   0.11     231.1      71.1  1.70    119
LowEntropy8K        8.0K   0.03   0.04     258.6     212.5  5.33    226
LargeText64K       64.0K   0.21   0.91     293.6      68.3  1.76    136
Overall Performance Score: 132.0
```

**What to Focus On:**
- **C(MB/s)** & **D(MB/s)**: Higher = better performance
- **Ratio**: Should stay roughly the same (compression quality)
- **Overall Score**: Single number to track improvement

### Typical Optimization Results
```bash
# Before optimization
Overall Score: 132.0

# After CLZ optimization  
Overall Score: 185.3  (+40% improvement)

# After NEON SIMD
Overall Score: 267.8  (+103% improvement)

# After cache optimization
Overall Score: 334.2  (+153% improvement)
```

## 🎯 Optimization Workflow

### Step-by-Step Process
1. **Establish Baseline**
   ```bash
   ./track_optimization.sh baseline
   ```
   
2. **Implement Single Optimization** (e.g., CLZ instructions)
   ```bash
   # Edit code to add CLZ optimization
   # Rebuild: gcc -I include/core -O3 -o huffman src/huffman_cli.c src/core/*.c
   ./track_optimization.sh test "clz-v1"
   ```

3. **Review Results**
   - Look for higher MB/s numbers
   - Check that compression ratios stayed stable
   - Note overall score improvement

4. **Implement Next Optimization** (e.g., NEON SIMD)
   ```bash
   # Add NEON SIMD code
   ./track_optimization.sh test "clz-neon-v1"
   ```

5. **Continue Iterating**
   ```bash
   ./track_optimization.sh test "clz-neon-cache-v1"
   ./track_optimization.sh test "clz-neon-cache-v2"
   ```

6. **Track Best Results**
   ```bash
   ./track_optimization.sh best
   ```

## 🔍 What Each Test Measures

### **StandardText8K** - Real-world Performance
- **Purpose**: Typical English text compression
- **Baseline**: ~230 MB/s compression, ~70 MB/s decompression
- **Optimization Target**: Text is most common use case

### **HighEntropy8K** - Worst-case Scenario  
- **Purpose**: Random data that can't compress well
- **Baseline**: ~70 MB/s compression, ~50 MB/s decompression
- **Optimization Target**: Ensure optimizations don't break on difficult data

### **LowEntropy8K** - Best-case Scenario
- **Purpose**: Highly repetitive data
- **Baseline**: ~260 MB/s compression, ~210 MB/s decompression  
- **Optimization Target**: Maximum possible performance

### **LargeText64K** - Scalability Test
- **Purpose**: Performance on larger datasets
- **Baseline**: ~290 MB/s compression, ~70 MB/s decompression
- **Optimization Target**: Ensure optimizations scale with size

## 💡 Pro Tips

### Getting Accurate Results
- **Use high iterations** (`-i 50`) for final measurements
- **Run multiple times** and average if results vary
- **Keep system load low** during testing

### Tracking Progress
- **Save important results**: Each test automatically saves to JSON
- **Use descriptive names**: `"clz-neon-v3"` not `"test1"`
- **Compare incrementally**: Test one optimization at a time

### Expected ARM64 Improvements
| Optimization | Expected Improvement |
|--------------|---------------------|
| CLZ Instructions | +30-50% |
| NEON SIMD | +100-200% |
| Cache Alignment | +20-40% |
| Combined | +200-400% |

### Interpreting Regressions
- **Small regression** (< 5%): Probably measurement noise
- **Large regression** (> 10%): Check for bugs in optimization
- **Ratio changes**: Compression quality affected (bad!)

## 🗂️ File Structure

```
├── regression_test              # Main test runner
├── track_optimization.sh       # Optimization tracker
├── generate_fixed_tests        # Test data generator
├── fixed_tests/                # Fixed test data (never changes)
│   ├── standard_text_8k.dat
│   ├── high_entropy_8k.dat
│   ├── low_entropy_8k.dat
│   ├── ascii_code_8k.dat
│   ├── binary_mixed_8k.dat
│   ├── large_text_64k.dat
│   └── frequency_test_4k.dat
└── optimization_results/       # Saved test results
    ├── baseline.json
    ├── baseline_20250804_173732.json
    └── clz-optimization_20250804_180245.json
```

## 📝 Example Session

```bash
# Day 1: Establish baseline
./track_optimization.sh baseline
# Result: Score 132.0

# Day 2: Add CLZ optimization
# ... edit code to use __builtin_clz() ...
./track_optimization.sh test "clz-v1"
# Result: Score 185.3 (+40% improvement!)

# Day 3: Add NEON SIMD  
# ... edit code to add SIMD parallel processing ...
./track_optimization.sh test "clz-neon-v1"
# Result: Score 267.8 (+103% improvement!)

# Day 4: Optimize cache alignment
./track_optimization.sh test "clz-neon-cache-v1"  
# Result: Score 334.2 (+153% improvement!)

# Check best results
./track_optimization.sh best
# Shows: clz-neon-cache-v1 is your best performer
```

---

**Use this system to precisely track your ARM64 optimization progress with consistent, reproducible measurements!**