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
    ${CMAKE_CURRENT_SOURCE_DIR}/include/core
)

# Core source files
set(CORE_SOURCES
    src/core/huffman_tree.c
    src/core/bit_stream.c
    src/core/decoder.c
    src/core/encoder.c
    src/core/file_format.c
    src/core/huffman_compress.c
    src/core/benchmark.c
    src/core/regression_test.c
)

# Main library
add_library(huffman_m4 STATIC ${CORE_SOURCES})

# Executables
add_executable(huffman src/huffman_cli.c)
add_executable(huffman_benchmark src/benchmark_runner.c)
add_executable(regression_test src/regression_runner.c)
add_executable(generate_fixed_tests src/generate_fixed_tests.c)

# Link libraries
target_link_libraries(huffman huffman_m4)
target_link_libraries(huffman_benchmark huffman_m4)
target_link_libraries(regression_test huffman_m4)

# Install targets
install(TARGETS huffman huffman_benchmark regression_test huffman_m4
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(DIRECTORY include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h"
)

# Generate compile_commands.json for IDE support
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Print configuration summary
message(STATUS "")
message(STATUS "M4-Optimized Huffman Compression")
message(STATUS "=================================")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C Standard: ${CMAKE_C_STANDARD}")
message(STATUS "System: ${CMAKE_SYSTEM_NAME}")
message(STATUS "Processor: ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "")