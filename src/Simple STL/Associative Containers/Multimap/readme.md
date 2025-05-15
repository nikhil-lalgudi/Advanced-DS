# `custom::multimap`

## Overview

`custom::multimap` is an associative container that stores key-value pairs in a sorted order, allowing duplicate keys. It is implemented as a Red-Black Tree, which provides efficient operations for insertion, deletion, and lookup. The elements are always kept sorted according to the key's comparison function (default is `std::less<Key>`).

## Key Features

*   **Duplicate Keys**: Unlike `map`, `multimap` allows multiple elements with the same key.
*   **Sorted Order**: Elements are always kept in sorted order according to the key's comparison function.
*   **Efficient Operations**: 
    *   Insertion: O(log n)
    *   Deletion: O(log n)
    *   Lookup: O(log n)
*   **Bidirectional Iteration**: Supports iterating forwards and backwards through the elements.
*   **Custom Comparator**: Allows custom comparison functions for key ordering.

## Implementation Details

The implementation uses a Red-Black Tree, which is a self-balancing binary search tree. This ensures that the tree remains balanced, providing O(log n) time complexity for all operations. The tree maintains the following properties:

1. Every node is either red or black.
2. The root is black.
3. All leaves (NIL) are black.
4. If a node is red, then both its children are black.
5. Every path from the root to any leaf contains the same number of black nodes.

## Basic Usage Examples

```cpp
#include "multimap.h"
#include <iostream>
#include <string>

int main() {
    // Default constructor
    custom::multimap<std::string, int> myMap;

    // Insert elements (including duplicates)
    myMap.insert({"apple", 1});
    myMap.insert({"banana", 2});
    myMap.insert({"apple", 3});  // Duplicate key allowed
    myMap.insert({"cherry", 4});
    myMap.insert({"apple", 5});  // Another duplicate key

    // Iterate through elements (they will be in sorted order by key)
    std::cout << "Map elements: ";
    for (const auto& [key, value] : myMap) {
        std::cout << key << ":" << value << " ";
    }
    std::cout << std::endl; // Output: Map elements: apple:1 apple:3 apple:5 banana:2 cherry:4

    // Count occurrences of a key
    std::cout << "Number of 'apple' entries: " << myMap.count("apple") << std::endl; // Output: 3
    std::cout << "Number of 'fig' entries: " << myMap.count("fig") << std::endl; // Output: 0

    // Check for key presence
    std::cout << "Contains 'banana'? " << (myMap.contains("banana") ? "Yes" : "No") << std::endl; // Output: Yes
    std::cout << "Contains 'fig'? " << (myMap.contains("fig") ? "Yes" : "No") << std::endl; // Output: No

    // Find an element
    auto it = myMap.find("apple");
    if (it != myMap.end()) {
        std::cout << "Found element: " << it->first << ":" << it->second << std::endl; // Output: Found element: apple:1
    }

    // Custom comparator example
    custom::multimap<std::string, int, std::greater<std::string>> reverseMap;
    reverseMap.insert({"apple", 1});
    reverseMap.insert({"banana", 2});
    reverseMap.insert({"apple", 3});  // Duplicate key allowed

    std::cout << "Reverse sorted map: ";
    for (const auto& [key, value] : reverseMap) {
        std::cout << key << ":" << value << " ";
    }
    std::cout << std::endl; // Output: Reverse sorted map: banana:2 apple:1 apple:3

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