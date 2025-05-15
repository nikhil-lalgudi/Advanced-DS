# `custom::multiset`

## Overview

`custom::multiset` is an associative container that stores elements in a sorted order, allowing duplicate elements. It is implemented as a Red-Black Tree, which provides efficient operations for insertion, deletion, and lookup. The elements are always kept sorted according to the comparison function (default is `std::less<T>`).

## Key Features

*   **Duplicate Elements**: Unlike `set`, `multiset` allows multiple elements with the same value.
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
#include "multiset.h"
#include <iostream>
#include <string>

int main() {
    // Default constructor
    custom::multiset<int> mySet;

    // Insert elements (including duplicates)
    mySet.insert(1);
    mySet.insert(2);
    mySet.insert(2);  // Duplicate allowed
    mySet.insert(3);
    mySet.insert(3);  // Another duplicate

    // Iterate through elements (they will be in sorted order)
    std::cout << "Set elements: ";
    for (const auto& value : mySet) {
        std::cout << value << " ";
    }
    std::cout << std::endl; // Output: Set elements: 1 2 2 3 3

    // Count occurrences of an element
    std::cout << "Number of 2s: " << mySet.count(2) << std::endl; // Output: 2
    std::cout << "Number of 4s: " << mySet.count(4) << std::endl; // Output: 0

    // Check for element presence
    std::cout << "Contains 3? " << (mySet.contains(3) ? "Yes" : "No") << std::endl; // Output: Yes
    std::cout << "Contains 4? " << (mySet.contains(4) ? "Yes" : "No") << std::endl; // Output: No

    // Find an element
    auto it = mySet.find(2);
    if (it != mySet.end()) {
        std::cout << "Found element: " << *it << std::endl; // Output: Found element: 2
    }

    // Custom comparator example
    custom::multiset<std::string, std::greater<std::string>> reverseSet;
    reverseSet.insert("apple");
    reverseSet.insert("banana");
    reverseSet.insert("apple");  // Duplicate allowed

    std::cout << "Reverse sorted set: ";
    for (const auto& value : reverseSet) {
        std::cout << value << " ";
    }
    std::cout << std::endl; // Output: Reverse sorted set: banana apple apple

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
*   Add support for range-based insert and erase operations 