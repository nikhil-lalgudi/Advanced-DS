# custom::deque - A Simplified C++17 Deque Implementation

This is a simplified implementation of the `std::deque` (double-ended queue) container from the C++ Standard Template Library (STL), implemented as `custom::deque`.

## Overview

A deque (pronounced "deck") is a sequence container that allows fast insertion and deletion at both its beginning and end. Unlike `std::vector`, which only allows efficient operations at the end, a deque is optimized for modifying both ends of the sequence.

This simplified implementation provides the core functionality of a deque using a circular buffer approach.

## Implementation Approach

Our implementation uses a simple circular buffer with the following characteristics:

- Single contiguous memory buffer that grows when needed
- O(1) insertions and removals at both ends
- O(1) random access via index
- Automatic resizing when capacity is reached

Unlike the standard library implementation (which typically uses multiple memory segments), this simplified version uses a single contiguous array for storage, with the front index wrapping around as needed.

## Files

- `deque.h` - The header-only implementation of `custom::deque`
- `dequetest.cpp` - A test suite for the implementation
- `README.md` - Documentation for the implementation

## Features

### Member Types

The implementation provides the basic member types:

```cpp
using value_type = T;
using reference = T&;
using const_reference = const T&;
using pointer = T*;
using const_pointer = const T*;
using size_type = size_t;
```

### Element Access

- `at(pos)` - Access element with bounds checking
- `operator[](pos)` - Access element without bounds checking
- `front()` - Access the first element
- `back()` - Access the last element

### Iterators

- `begin()`, `end()` - Basic forward iterators

### Capacity

- `empty()` - Check if the container is empty
- `size()` - Return the number of elements

### Modifiers

- `push_back(value)` - Add element at the end
- `push_front(value)` - Add element at the beginning
- `pop_back()` - Remove element from the end
- `pop_front()` - Remove element from the beginning
- `clear()` - Clear all elements

### Constructors/Destructors

- Default constructor
- Constructor with count and value
- Copy constructor
- Move constructor
- Destructor

## How it Works

### Circular Buffer

The core of the implementation is a circular buffer. Here's how it works:

1. We maintain a buffer of elements with `capacity_` size.
2. `front_index_` tracks the index of the first element.
3. Elements are stored contiguously, wrapping around from the end to the beginning.
4. The logical position `i` corresponds to physical position `(front_index_ + i) % capacity_`.

### Growth Strategy

When the deque is full and a new element is added:

1. A new buffer is allocated with twice the capacity.
2. All elements are moved to the new buffer, starting at index 0.
3. The `front_index_` is reset to 0.
4. The old buffer is deleted.

### Iterator Implementation

A basic iterator is included that supports:
- Dereferencing (`operator*`)
- Incrementing (`operator++`)
- Decrementing (`operator--`)
- Comparison (`operator==`, `operator!=`)

This allows for standard algorithm compatibility.

## Usage

Include the header:

```cpp
#include "deque.h"
```

Basic usage:

```cpp
// Create a deque
custom::deque<int> dq;

// Insert elements at both ends
dq.push_back(30);
dq.push_front(10);
dq.push_back(40);
dq.push_front(5);

// Access elements
int first = dq.front();  // 5
int last = dq.back();    // 40
int second = dq[1];      // 10

// Remove elements
dq.pop_front();  // Removes 5
dq.pop_back();   // Removes 40

// Iterate through elements
for (const auto& elem : dq) {
    std::cout << elem << " ";  // Outputs: 10 30
}
```

## Limitations

This is a simplified implementation with some limitations:

1. No reverse iterators or const iterators
2. Limited algorithm support due to the basic iterators
3. No custom allocator support
4. No `emplace` or `insert` operations beyond front/back
5. No `erase` operations
6. Not as memory-efficient as the standard library implementation
7. No bounds checking in some methods

## Testing

The test suite in `dequetest.cpp` verifies the core functionality:

- Basic operations (push, pop, access)
- Iterator functionality
- Constructor behaviors
- Growth and resizing
- Algorithm compatibility
- Different element types

### Running the Tests

Compile and run the test suite:

```bash
g++ -std=c++17 -o dequetest dequetest.cpp
./dequetest
```

## Implementation Notes

1. **Circular Buffer**: This approach allows O(1) insertions and removals at both ends by using modular arithmetic to map logical indices to physical buffer positions.

2. **Growth Strategy**: Doubling the capacity when needed provides amortized O(1) push operations, similar to `std::vector`.

3. **Memory Management**: Unlike `std::deque`, which typically uses multiple memory segments, this implementation uses a single contiguous buffer for simplicity.

## References

- [C++ Reference: std::deque](https://en.cppreference.com/w/cpp/container/deque)
- [Circular Buffer Data Structure](https://en.wikipedia.org/wiki/Circular_buffer) 