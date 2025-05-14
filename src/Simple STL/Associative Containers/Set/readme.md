# `custom::set`

## Overview

`custom::set` is an associative container that stores unique elements in a sorted order. It is implemented as a Red-Black Tree, which provides efficient operations for insertion, deletion, and lookup. The elements are always kept sorted according to a comparison function (default is `std::less<Key>`).

## Key Features

*   **Unique Elements**: Each element in the set must be unique.
*   **Sorted Order**: Elements are always kept in sorted order according to the comparison function.
*   **Efficient Operations**: 
    *   Insertion: O(log n)
    *   Deletion: O(log n)
    *   Lookup: O(log n)
*   **Bidirectional Iteration**: Supports iterating forwards and backwards through the elements.
*   **Custom Comparator**: Allows custom comparison functions for element ordering.

## Implementation Details

The implementation uses a Red-Black Tree, which is a self-balancing binary search tree. This ensures that the tree remains balanced, providing O(log n) time complexity for all operations. The tree maintains the following properties:

1. Every node is either red or black.
2. The root is black.
3. All leaves (NIL) are black.
4. If a node is red, then both its children are black.
5. Every path from the root to any leaf contains the same number of black nodes.

## Basic Usage Examples

```cpp
#include "set.h"
#include <iostream>
#include <string>

int main() {
    // Default constructor
    custom::set<int> mySet;

    // Insert elements
    mySet.insert(10);
    mySet.insert(20);
    mySet.insert(5);
    mySet.insert(20); // Duplicate, will be ignored

    // Iterate through elements (they will be in sorted order)
    std::cout << "Set elements: ";
    for (int val : mySet) {
        std::cout << val << " ";
    }
    std::cout << std::endl; // Output: Set elements: 5 10 20

    // Check for element presence
    std::cout << "Contains 10? " << (mySet.contains(10) ? "Yes" : "No") << std::endl; // Output: Yes
    std::cout << "Contains 15? " << (mySet.contains(15) ? "Yes" : "No") << std::endl; // Output: No

    // Find an element
    auto it = mySet.find(5);
    if (it != mySet.end()) {
        std::cout << "Found element: " << *it << std::endl; // Output: Found element: 5
    }

    // Erase an element
    size_t erased = mySet.erase(10);
    std::cout << "Erased " << erased << " element(s)" << std::endl; // Output: Erased 1 element(s)

    // Custom comparator example
    custom::set<std::string, std::greater<std::string>> reverseSet;
    reverseSet.insert("apple");
    reverseSet.insert("banana");
    reverseSet.insert("cherry");

    std::cout << "Reverse sorted set: ";
    for (const auto& str : reverseSet) {
        std::cout << str << " ";
    }
    std::cout << std::endl; // Output: Reverse sorted set: cherry banana apple

    return 0;
}
```

## Implemented Features

*   Constructors:
    *   Default constructor
    *   Constructor with custom comparator
    *   Copy constructor
    *   Move constructor
    *   Initializer list constructor
*   Destructor
*   Assignment operators:
    *   Copy assignment
    *   Move assignment
    *   Initializer list assignment
*   Iterators:
    *   `begin()`, `end()`
    *   Bidirectional iteration
*   Capacity:
    *   `empty()`
    *   `size()`
*   Modifiers:
    *   `clear()`
    *   `insert()`
    *   `erase()`
*   Lookup:
    *   `find()`
    *   `count()`
    *   `contains()`

## Future Enhancements

*   Complete implementation of `erase()` operation
*   Add support for reverse iterators
*   Implement `emplace()` and `emplace_hint()` for in-place construction
*   Add support for custom allocators
*   Implement comparison operators (`==`, `!=`, `<`, `<=`, `>`, `>=`)
*   Add support for range-based operations
*   Implement `lower_bound()`, `upper_bound()`, and `equal_range()`
*   Add support for node-based operations (extract, merge) 