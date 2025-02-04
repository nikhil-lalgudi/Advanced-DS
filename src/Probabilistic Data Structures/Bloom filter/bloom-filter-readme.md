# High-Performance Bloom Filter Implementation in C++

A modern, cache-friendly, and highly optimized Bloom filter implementation designed for performance-critical applications. This implementation focuses on minimal memory usage while maintaining exceptional query performance through careful optimization and modern C++ practices.

## Overview

A Bloom filter is a space-efficient probabilistic data structure used to test whether an element is a member of a set. False positives are possible, but false negatives are not. This implementation provides an optimal balance between memory usage and performance, automatically calculating the ideal number of hash functions and bit array size based on the expected number of elements and desired false positive rate.

## Features

- **Highly Optimized Performance**
  - Bit-level operations for all set/test operations
  - Double hashing technique to minimize hash calculations
  - 64-bit aligned storage for efficient memory access
  - Optimized MurmurHash3 implementation
  - All critical methods marked as `inline` and `noexcept`
  - SIMD-friendly data layout

- **Memory Efficient**
  - Automatic calculation of optimal parameters
  - Compact bit storage using `std::vector<uint64_t>`
  - Memory aligned to 64-bit boundaries
  - Minimal memory overhead

- **Modern C++ Design**
  - Header-only option available
  - Complete `noexcept` guarantee for core operations
  - RAII-compliant
  - Template support for any data type
  - `[[nodiscard]]` attributes for better error prevention
  - Const-correct interface

## Installation

This is a header-only library by default. Simply include `bloom.h` in your project. If you prefer separate compilation, you can also use the provided implementation file `bloom.cpp`.

### Header-Only Usage
```cpp
#include "bloom_filter.hpp"
```

### Separate Compilation
```cpp
// In your source file
#include "bloom_filter.cpp"
```

## Usage

### Basic Usage

```cpp
#include "bloom_filter.hpp"

// Create a Bloom filter expecting 1000 elements with 1% false positive rate
BloomFilter filter(1000, 0.01);

// Insert elements
int value = 42;
filter.insert(value);

// Check for membership
bool contains = filter.contains(value);  // Returns true

// Clear the filter
filter.clear();
```

### Custom Types

```cpp
struct CustomType {
    int x;
    double y;
};

BloomFilter filter(1000, 0.01);
CustomType custom{1, 2.0};
filter.insert(custom);
bool hasCustom = filter.contains(custom);
```

### Size and Status Checking

```cpp
BloomFilter filter(1000, 0.01);

// Get the size in bits
size_t bitSize = filter.size();

// Get the number of hash functions
size_t hashCount = filter.hashCount();

// Check if the filter is empty
bool empty = filter.isEmpty();
```

## Performance Considerations

- **Memory Access**: The implementation uses 64-bit aligned storage to minimize cache misses and ensure efficient memory access patterns.
- **Hash Functions**: Uses an optimized MurmurHash3 implementation with double hashing technique to reduce the number of hash calculations needed.
- **Bit Operations**: All bit manipulations are optimized using bit-level operations and modern C++ bit manipulation features.
- **Cache Efficiency**: Data structure layout is designed to maximize cache line utilization.

## Implementation Details

### Optimal Parameters

The filter automatically calculates the optimal number of hash functions (k) and bit array size (m) based on the expected number of elements (n) and desired false positive rate (p) using the following formulas:

```
m = -n * ln(p) / (ln(2)^2)
k = ln(2) * m / n
```

### Hash Function Implementation

The implementation uses MurmurHash3 with the following optimizations:
- Double hashing technique to generate multiple hash values
- Optimized bit mixing for better distribution
- SIMD-friendly memory access patterns

## Time Complexity

- Insertion: O(k) where k is the number of hash functions
- Lookup: O(k) where k is the number of hash functions
- Clear: O(m/64) where m is the size of the bit array

## Space Complexity

The space usage is approximately `(-n * ln(p) / (ln(2)^2)` bits, rounded up to the nearest multiple of 64 bits, plus a small constant overhead for the class members.

## Requirements

- C++17 or later
- Support for 64-bit integers
- Standard library support for `<vector>`, `<random>`, and `<bit>`

## License
This implementation is provided under the [MIT License](LICENSE). See the LICENSE file for details.

## Further Optimizations

Possible future improvements:
1. SIMD operations for bulk insertions and queries
2. Hardware-specific optimizations
3. Thread-safe operations
4. Compressed storage for sparse filters
5. Support for custom hash functions
6. Serialization support

## References

1. Burton H. Bloom. "Space/Time Trade-offs in Hash Coding with Allowable Errors" (1970)
2. MurmurHash3 by Austin Appleby
3. "Less Hashing, Same Performance: Building a Better Bloom Filter" by Kirsch and Mitzenmacher
