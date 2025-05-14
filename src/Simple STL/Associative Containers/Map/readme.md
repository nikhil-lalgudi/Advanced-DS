# `custom::map`

## Overview

`custom::map` is an associative container that stores key-value pairs in a sorted order. It is implemented as a Red-Black Tree, which provides efficient operations for insertion, deletion, and lookup. The elements are always kept sorted according to the key's comparison function (default is `std::less<Key>`).

## Key Features

*   **Unique Keys**: Each key in the map must be unique.
*   **Sorted Order**: Elements are always kept in sorted order according to the key's comparison function.
*   **Efficient Operations**: 
    *   Insertion: O(log n)
    *   Deletion: O(log n)
    *   Lookup: O(log n)
*   **Bidirectional Iteration**: Supports iterating forwards and backwards through the elements.
*   **Custom Comparator**: Allows custom comparison functions for key ordering.
*   **Direct Element Access**: Provides `operator[]` and `at()` for accessing values by key.

## Implementation Details

The implementation uses a Red-Black Tree, which is a self-balancing binary search tree. This ensures that the tree remains balanced, providing O(log n) time complexity for all operations. The tree maintains the following properties:

1. Every node is either red or black.
2. The root is black.
3. All leaves (NIL) are black.
4. If a node is red, then both its children are black.
5. Every path from the root to any leaf contains the same number of black nodes.

## Basic Usage Examples

```cpp
#include "map.h"
#include <iostream>
#include <string>

int main() {
    // Default constructor
    custom::map<std::string, int> myMap;

    // Insert elements
    myMap.insert({"apple", 1});
    myMap.insert({"banana", 2});
    myMap.insert({"cherry", 3});
    myMap.insert({"banana", 4}); // Duplicate key, will be ignored

    // Access elements using operator[]
    myMap["date"] = 4;
    std::cout << "Value of 'date': " << myMap["date"] << std::endl; // Output: 4

    // Access elements using at()
    try {
        std::cout << "Value of 'apple': " << myMap.at("apple") << std::endl; // Output: 1
        std::cout << "Value of 'fig': " << myMap.at("fig") << std::endl; // Throws std::out_of_range
    } catch (const std::out_of_range& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    // Iterate through elements (they will be in sorted order by key)
    std::cout << "Map elements: ";
    for (const auto& [key, value] : myMap) {
        std::cout << key << ":" << value << " ";
    }
    std::cout << std::endl; // Output: Map elements: apple:1 banana:2 cherry:3 date:4

    // Check for key presence
    std::cout << "Contains 'banana'? " << (myMap.contains("banana") ? "Yes" : "No") << std::endl; // Output: Yes
    std::cout << "Contains 'fig'? " << (myMap.contains("fig") ? "Yes" : "No") << std::endl; // Output: No

    // Find an element
    auto it = myMap.find("cherry");
    if (it != myMap.end()) {
        std::cout << "Found element: " << it->first << ":" << it->second << std::endl; // Output: Found element: cherry:3
    }

    // Erase an element
    size_t erased = myMap.erase("banana");
    std::cout << "Erased " << erased << " element(s)" << std::endl; // Output: Erased 1 element(s)

    // Custom comparator example
    custom::map<std::string, int, std::greater<std::string>> reverseMap;
    reverseMap.insert({"apple", 1});
    reverseMap.insert({"banana", 2});
    reverseMap.insert({"cherry", 3});

    std::cout << "Reverse sorted map: ";
    for (const auto& [key, value] : reverseMap) {
        std::cout << key << ":" << value << " ";
    }
    std::cout << std::endl; // Output: Reverse sorted map: cherry:3 banana:2 apple:1

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
*   Element access:
    *   `operator[]`
    *   `at()`
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