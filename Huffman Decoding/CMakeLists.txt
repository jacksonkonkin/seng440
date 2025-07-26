cmake_minimum_required(VERSION 3.20)
project(HuffmanDecoderM4 VERSION 1.0.0 LANGUAGES C)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Set build type if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# ARM64 specific configuration for Apple Silicon
if(APPLE AND CMAKE_SYSTEM_PROCESSOR MATCHES "arm64")
    message(STATUS "Building for Apple Silicon M4")
    
    # ARM64 optimization flags for M4
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -arch arm64")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3 -DNDEBUG")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mcpu=apple-m1")  # Use M1 target until M4 support
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mtune=native")
    
    # Enable ARM64 specific optimizations
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -ffast-math")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -funroll-loops")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fvectorize")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -flto")
    
    # Cache optimization flags
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -falign-functions=64")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -falign-loops=64")
    
endif()

# Debug flags
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -O0")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -Wextra -Wpedantic")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fsanitize=address")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fsanitize=undefined")

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/core
    ${CMAKE_CURRENT_SOURCE_DIR}/include/optimization
    ${CMAKE_CURRENT_SOURCE_DIR}/include/arm64
    ${CMAKE_CURRENT_SOURCE_DIR}/include/simd
)

# Source files
set(CORE_SOURCES
    src/core/huffman_tree.c
    src/core/bit_stream.c
    src/core/decoder.c
)

# Main library
add_library(huffman_decoder_m4 STATIC
    ${CORE_SOURCES}
)

# Main executable
add_executable(huffman_decoder
    src/main.c
)

target_link_libraries(huffman_decoder huffman_decoder_m4)

# Testing
enable_testing()

# Find or build test framework (using simple custom tests for now)
set(TEST_SOURCES
    tests/test_huffman_tree.cpp
    tests/test_bit_stream.cpp
    tests/test_lookup_table.cpp
    tests/test_decoder.cpp
    tests/test_arm64_ops.cpp
    tests/test_simd_ops.cpp
    tests/test_runner.cpp
)

add_executable(run_tests ${TEST_SOURCES})
target_link_libraries(run_tests huffman_decoder_m4)

# Benchmarks
set(BENCHMARK_SOURCES
    benchmarks/benchmark_decoder.cpp
    benchmarks/benchmark_arm64.cpp
    benchmarks/benchmark_simd.cpp
    benchmarks/benchmark_runner.cpp
)

add_executable(run_benchmarks ${BENCHMARK_SOURCES})
target_link_libraries(run_benchmarks huffman_decoder_m4)

# Custom targets for common tasks
add_custom_target(test_all
    COMMAND run_tests
    DEPENDS run_tests
    COMMENT "Running all tests"
)

add_custom_target(benchmark_all
    COMMAND run_benchmarks
    DEPENDS run_benchmarks
    COMMENT "Running all benchmarks"
)

# Install targets
install(TARGETS huffman_decoder huffman_decoder_m4
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(DIRECTORY include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)

# Generate compile_commands.json for IDE support
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Print configuration summary
message(STATUS "")
message(STATUS "Configuration Summary:")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  C Standard: ${CMAKE_C_STANDARD}")
message(STATUS "  System: ${CMAKE_SYSTEM_NAME}")
message(STATUS "  Processor: ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "  C Flags: ${CMAKE_C_FLAGS}")
message(STATUS "  C Flags (Release): ${CMAKE_C_FLAGS_RELEASE}")
message(STATUS "  C Flags (Debug): ${CMAKE_C_FLAGS_DEBUG}")
message(STATUS "")