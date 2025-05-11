# `custom::vector`

## Overview

`custom::vector` is a sequential container that implements a dynamic array. It stores elements in contiguous memory locations, allowing for fast random access (O(1) time complexity). When the vector's capacity is exhausted and new elements are added, it typically reallocates a larger block of memory and moves/copies existing elements to the new location.

This implementation includes basic allocator support using `std::allocator_traits`.

## Key Features

*   **Dynamic Size**: The vector can grow or shrink as needed.
*   **Random Access**: Elements can be accessed directly by their index in O(1) time using `operator[]` or `at()`.
*   **Contiguous Storage**: Elements are stored contiguously, which can be beneficial for cache performance and interoperability with C-style arrays.
*   **Efficient End Insertions/Deletions (Amortized)**: Adding or removing elements at the end (`push_back`, `pop_back`) is amortized O(1) on average, though individual operations can be O(n) if a reallocation occurs.

## Disadvantages/Considerations

*   **Costly Insertions/Deletions in the Middle**: Inserting or erasing elements at positions other than the end requires shifting subsequent elements, which is an O(n) operation.
*   **Reallocation Overhead**: When the vector's capacity is exceeded, a reallocation occurs, which involves allocating new memory, moving/copying elements, and deallocating old memory. This can be a time-consuming operation.
*   **Iterator Invalidation**: Reallocations invalidate all iterators, pointers, and references to elements. Insertions or deletions may invalidate iterators, pointers, and references at or after the point of modification.

## Basic Usage Examples

```cpp
#include "vector.h"
#include <iostream>

int main() {
    // Default constructor
    custom::vector<int> myVector;

    // Add elements using push_back
    myVector.push_back(10); // myVector: {10}, size: 1, capacity: >=1
    myVector.push_back(20); // myVector: {10, 20}, size: 2, capacity: >=2
    myVector.push_back(30); // myVector: {10, 20, 30}, size: 3, capacity: >=3

    std::cout << "Initial vector: ";
    for (size_t i = 0; i < myVector.size(); ++i) {
        std::cout << myVector[i] << " ";
    }
    std::cout << "(Size: " << myVector.size() << ", Capacity: " << myVector.capacity() << ")" << std::endl;
    // Output: Initial vector: 10 20 30 (Size: 3, Capacity: ...)

    // Element access
    std::cout << "Element at index 1: " << myVector[1] << std::endl;    // Output: 20
    std::cout << "Front element: " << myVector.front() << std::endl;      // Output: 10
    std::cout << "Back element: " << myVector.back() << std::endl;       // Output: 30

    // Insert an element
    custom::vector<int>::iterator it = myVector.begin() + 1; // iterator to 20
    myVector.insert(it, 15); // myVector: {10, 15, 20, 30}
    
    std::cout << "Vector after inserting 15: ";
    for (int val : myVector) {
        std::cout << val << " ";
    }
    std::cout << std::endl; // Output: Vector after inserting 15: 10 15 20 30

    // Erase an element
    it = myVector.begin() + 2; // iterator to 20
    it = myVector.erase(it);   // erases 20, it now points to 30
    // myVector: {10, 15, 30}

    std::cout << "Vector after erasing 20 (iterator points to " << *it << "): ";
    for (int val : myVector) {
        std::cout << val << " ";
    }
    std::cout << std::endl; // Output: Vector after erasing 20 (iterator points to 30): 10 15 30

    // Pop an element from the back
    myVector.pop_back();  // myVector: {10, 15}

    std::cout << "Vector after pop_back: ";
    for (int val : myVector) {
        std::cout << val << " ";
    }
    std::cout << std::endl; // Output: Vector after pop_back: 10 15

    // Capacity management
    std::cout << "Size: " << myVector.size() << ", Capacity: " << myVector.capacity() << std::endl;
    myVector.reserve(100);
    std::cout << "After reserve(100) - Size: " << myVector.size() << ", Capacity: " << myVector.capacity() << std::endl;
    myVector.shrink_to_fit();
    std::cout << "After shrink_to_fit() - Size: " << myVector.size() << ", Capacity: " << myVector.capacity() << std::endl;

    // Clear the vector
    myVector.clear();
    std::cout << "Size after clear: " << myVector.size() << ", Capacity: " << myVector.capacity() << std::endl;
    std::cout << "Is empty after clear? " << (myVector.empty() ? "Yes" : "No") << std::endl;

    return 0;
}
```

## Implemented Features

*   Allocator support (`std::allocator_traits`)
*   Constructors (default, allocator-aware, count-value, range, copy, move, initializer_list)
*   Destructor
*   Assignment operators (copy, move, initializer_list)
*   `assign` methods (count-value, range)
*   Element access (`at`, `operator[]`, `front`, `back`, `data`)
*   Iterators (raw pointers as iterators, reverse iterators: `begin`, `end`, `rbegin`, `rend`, `cbegin`, `cend`, `crbegin`, `crend`)
*   Capacity (`empty`, `size`, `max_size`, `reserve`, `capacity`, `shrink_to_fit`)
*   Modifiers:
    *   `clear`
    *   `insert` (single value, single rvalue, fill, range, initializer_list)
    *   `erase` (single position, range)
    *   `push_back` (lvalue, rvalue)
    *   `pop_back`
    *   `resize` (with default value, with provided value)
    *   `swap`
*   Non-member comparison operators (`==`, `!=`, `<`, `<=`, `>`, `>=`)
*   Non-member `swap`

## Future Enhancements (Potentially)

*   Full `emplace` and `emplace_back` methods with perfect forwarding and allocator awareness.
*   More sophisticated allocator handling, especially for stateful allocators and propagation.
*   Exception safety guarantees for operations like `insert` and `resize` could be further refined (e.g., strong exception guarantee where appropriate). 