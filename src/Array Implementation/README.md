# custom::array - A C++17 Implementation of std::array

This is a complete, standards-compliant implementation of the `std::array` container from the C++ Standard Template Library (STL), implemented as `custom::array`.

## Overview

The `std::array` container is an aggregate type that encapsulates a fixed-size array with compile-time size. It combines the performance and accessibility of a C-style array with the interface of a standard container.

This implementation provides all the functionality of `std::array` with C++17 features, including:

- All the member functions and types defined in the standard
- Complete iterator support (including reverse iterators)
- Tuple-like interface for structured bindings
- C++17 deduction guides
- Comprehensive `constexpr` support

## Files

- `array.h` - The header-only implementation of `custom::array`
- `arraytest.cpp` - A comprehensive test suite for the implementation
- `README.md` - Documentation for the implementation

## Features

### Member Types

The implementation provides all the standard member types:

```cpp
using value_type             = T;
using size_type              = std::size_t;
using difference_type        = std::ptrdiff_t;
using reference              = value_type&;
using const_reference        = const value_type&;
using pointer                = value_type*;
using const_pointer          = const value_type*;
using iterator               = pointer;
using const_iterator         = const_pointer;
using reverse_iterator       = std::reverse_iterator<iterator>;
using const_reverse_iterator = std::reverse_iterator<const_iterator>;
```

### Element Access

- `at(pos)` - Access element with bounds checking
- `operator[](pos)` - Access element without bounds checking
- `front()` - Access the first element
- `back()` - Access the last element
- `data()` - Direct access to the underlying array

### Iterators

- `begin()`, `cbegin()` - Iterator to the beginning
- `end()`, `cend()` - Iterator to the end
- `rbegin()`, `crbegin()` - Reverse iterator to the reversed beginning
- `rend()`, `crend()` - Reverse iterator to the reversed end

### Capacity

- `empty()` - Check if the container is empty
- `size()` - Return the number of elements
- `max_size()` - Return the maximum possible number of elements

### Operations

- `fill(value)` - Fill the array with the given value
- `swap(other)` - Swap contents with another array

### Non-member Functions

- Comparison operators: `==`, `!=`, `<`, `<=`, `>`, `>=`
- Non-member `swap()`
- Tuple-like interface: `std::get<I>`, `std::tuple_size`, `std::tuple_element`
- Custom helpers: `custom::get<I>`, `custom::size`

### C++17 Features

- `[[nodiscard]]` attributes on appropriate methods
- `constexpr` for all applicable functions
- Deduction guides for class template argument deduction (CTAD)
- Structured binding support
- `std::is_nothrow_swappable_v` type trait

## Special Handling for Zero-sized Arrays

The implementation includes special handling for zero-sized arrays (`custom::array<T, 0>`), as required by the standard:

- Element access functions are properly defined (but provide undefined behavior or throw as appropriate)
- Iterators work correctly, with `begin() == end()`
- Capacity functions properly return `0` or `true` for `empty()`

## Usage

Include the header:

```cpp
#include "array.h"
```

Create and use arrays:

```cpp
// Create an array
custom::array<int, 5> arr = {1, 2, 3, 4, 5};

// Access elements
int third = arr[2];
int last = arr.back();

// Use with algorithms
std::sort(arr.begin(), arr.end());

// With C++17 deduction guides
custom::array arr2{3.14, 2.71, 1.41};  // deduces custom::array<double, 3>

// With structured binding (C++17)
custom::array<int, 3> point = {10, 20, 30};
auto [x, y, z] = point;
```

## Testing

The test suite in `arraytest.cpp` verifies all aspects of the implementation:

- Basic functionality (creation, access)
- Iterators (all types)
- Capacity methods
- Operations (fill, swap)
- Comparison operators
- Integration with STL algorithms
- Tuple-like interface
- Zero-sized arrays behavior
- C++17 deduction guides
- Compatibility with different types
- Constexpr functionality
- Comparison with std::array behavior

### Running the Tests

Compile and run the test suite:

```bash
g++ -std=c++17 -o arraytest arraytest.cpp
./arraytest
```

## Implementation Notes

1. **Design Decisions**:
   - All operations that could be `constexpr` are marked as such
   - The public interface matches `std::array` exactly
   - The implementation maintains direct access to the underlying array via `elements_`

2. **Zero-sized Array Behavior**:
   - We provide a specialized template for zero-sized arrays
   - For some undefined behavior operations (like `operator[]`), we provide "safer" implementations that don't crash

3. **C++17 Features**:
   - [`[[nodiscard]]`](https://en.cppreference.com/w/cpp/language/attributes/nodiscard) to prevent accidental value discarding
   - [Deduction guides](https://en.cppreference.com/w/cpp/language/class_template_argument_deduction) for cleaner syntax
   - Full compatibility with [structured bindings](https://en.cppreference.com/w/cpp/language/structured_binding)

## References

- [C++ Reference: std::array](https://en.cppreference.com/w/cpp/container/array)
- [C++17 Standard: 26.3.7 Class template array](https://www.open-std.org/jtc1/sc22/wg21/) 