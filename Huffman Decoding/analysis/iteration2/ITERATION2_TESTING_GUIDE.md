# Iteration 2: ARM64 Optimization Testing Guide

This guide covers all available test files and executables for Iteration 2 ARM64 optimizations.

## Available Test Files

### Fixed Test Suite (7 standardized tests)
Located in `fixed_tests/` directory:

| File | Size | Purpose | Compression Profile |
|------|------|---------|-------------------|
| `standard_text_8k.dat` | 8KB | Typical English text | Medium compression |
| `high_entropy_8k.dat` | 8KB | Random/encrypted data | Worst case (no compression) |
| `low_entropy_8k.dat` | 8KB | Repetitive patterns | Best case (high compression) |
| `ascii_code_8k.dat` | 8KB | Source code structures | Medium compression |
| `binary_mixed_8k.dat` | 8KB | Mixed binary patterns | Low-medium compression |
| `large_text_64k.dat` | 64KB | Large text scalability | Medium compression, performance test |
| `frequency_test_4k.dat` | 4KB | Specific frequency distribution | High compression |

## Available Executables

### Compression/Decompression Tools
1. **`huffman_baseline`** - Original unoptimized version
2. **`huffman_cli_iteration2`** - Best performing CLZ-optimized version (recommended)
3. **`huffman_clz`** - CLZ-only optimizations
4. **`huffman_optimized`** - Full optimizations (has overhead)

### Regression Testing Tools
1. **`regression_test_baseline`** - Baseline performance measurement
2. **`huffman_iteration2`** - Final Iteration 2 regression tester (recommended)
3. **`regression_test_clz`** - CLZ-only performance testing
4. **`regression_test_optimized`** - Full optimization testing

### Benchmarking Tools
- **`run_comprehensive_benchmark.sh`** - Complete comparison script

## Quick Testing Commands

### 1. Run Single Performance Test
```bash
# Test the best Iteration 2 version
./huffman_iteration2

# Expected output: Score around 141.3 (+8.3% improvement)
```

### 2. Compare All Optimization Phases
```bash
# Run comprehensive comparison
./run_comprehensive_benchmark.sh

# This will test baseline, CLZ-only, and full optimizations
```

### 3. Test Individual Files
```bash
# Compress a test file
./huffman_cli_iteration2 -c fixed_tests/standard_text_8k.dat standard_text_8k.huf

# Decompress and verify
./huffman_cli_iteration2 -d standard_text_8k.huf standard_text_8k_restored.dat

# Compare original vs restored
diff fixed_tests/standard_text_8k.dat standard_text_8k_restored.dat
# (Should show no differences)
```

### 4. Performance Comparison Test
```bash
# Test baseline
./regression_test_baseline > baseline_score.txt

# Test Iteration 2
./huffman_iteration2 > iteration2_score.txt

# Compare scores
echo "Baseline: $(tail -n 3 baseline_score.txt | grep 'Current Score:' | awk '{print $3}')"
echo "Iteration 2: $(tail -n 3 iteration2_score.txt | grep 'Current Score:' | awk '{print $3}')"
```

## Expected Performance Results

### Baseline vs Iteration 2 Comparison
```
Version                Score    Improvement    Key Metrics
--------------------------------------------------------
Baseline              130.5     -             Reference
Iteration 2 (CLZ)     141.3     +8.3%        Best version
```

### Individual Test Performance
| Test | Baseline D(MB/s) | Iteration 2 D(MB/s) | Improvement |
|------|------------------|---------------------|-------------|
| StandardText8K | 70.1 | 77.1 | +10.0% |
| LargeText64K | 73.5 | 78.6 | +6.9% |
| LowEntropy8K | 108.6 | 111.6 | +2.8% |

## Automated Testing Scripts

### Quick Performance Check
```bash
#!/bin/bash
echo "Testing Iteration 2 ARM64 Optimizations..."
SCORE=$(./huffman_iteration2 | tail -n 3 | grep 'Current Score:' | awk '{print $3}')
echo "Current Score: $SCORE"
if (( $(echo "$SCORE > 140" | bc -l) )); then
    echo "✅ Performance target met (>140)"
else
    echo "⚠️ Performance below target (<140)"
fi
```

### Correctness Verification
```bash
#!/bin/bash
echo "Verifying compression/decompression correctness..."
for file in fixed_tests/*.dat; do
    basename=$(basename "$file" .dat)
    ./huffman_cli_iteration2 -c "$file" "${basename}_test.huf"
    ./huffman_cli_iteration2 -d "${basename}_test.huf" "${basename}_restored.dat"
    if diff "$file" "${basename}_restored.dat" > /dev/null; then
        echo "✅ $basename: Correct"
    else
        echo "❌ $basename: FAILED"
    fi
    rm -f "${basename}_test.huf" "${basename}_restored.dat"
done
```

## Assembly Analysis Files

For developers interested in the ARM64 optimizations:

### Assembly Comparison Files
Located in `assembly_comparison/`:
- `bit_stream_baseline.s` - Original assembly
- `bit_stream_clz.s` - CLZ optimized assembly
- `bit_stream_final.s` - Full optimization assembly
- `decoder_baseline.s` - Original decoder assembly
- `decoder_clz.s` - CLZ optimized decoder
- `decoder_final.s` - Full optimization decoder

### View Assembly Differences
```bash
# Compare baseline vs CLZ optimizations
diff assembly_comparison/bit_stream_baseline.s assembly_comparison/bit_stream_clz.s

# Count ARM64 specific instructions
grep -c "clz\|csel\|rbit\|rev" assembly_comparison/bit_stream_*.s
```

## Performance Validation Criteria

### Acceptance Tests
1. **Functional Correctness**: All test files must compress/decompress perfectly
2. **Performance Improvement**: Score must be > 140 (vs baseline ~130)
3. **No Regressions**: No individual test should perform worse than baseline
4. **Stability**: Results should be consistent across multiple runs (< 5% variance)

### Success Metrics
- ✅ **Achieved**: 8.3% overall performance improvement
- ✅ **Achieved**: Perfect functional correctness
- ✅ **Achieved**: Stable, repeatable results
- ✅ **Achieved**: ARM64 CLZ instructions successfully utilized

## Iteration 3 Preparation

The test infrastructure is ready for Iteration 3 NEON SIMD optimizations. Expected performance targets:
- **Target Score**: 250-400 (2-3x improvement)  
- **Key Focus**: Multi-stream SIMD processing
- **Test Files**: Same fixed test suite for consistency

---

**Recommended Command for Quick Test**: `./huffman_iteration2`  
**Recommended Version for Production**: `huffman_cli_iteration2`