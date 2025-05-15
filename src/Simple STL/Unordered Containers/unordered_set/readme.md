# `custom::unordered_set`

## Overview

`custom::unordered_set` is an associative container that stores unique elements in an unordered manner. It is implemented using a hash table with separate chaining, which provides efficient operations for insertion, deletion, and lookup. The elements are not kept in any particular order, but instead are organized based on their hash values.

## Key Features

*   **Unique Elements**: Each element in the set must be unique.
*   **Unordered Storage**: Elements are stored based on their hash values, not in any sorted order.
*   **Efficient Operations**: 
    *   Average case:
        *   Insertion: O(1)
        *   Deletion: O(1)
        *   Lookup: O(1)
    *   Worst case (with poor hash function):
        *   Insertion: O(n)
        *   Deletion: O(n)
        *   Lookup: O(n)
*   **Custom Hash Function**: Allows custom hash functions for element types.
*   **Custom Equality**: Allows custom equality comparison for elements.
*   **Load Factor Control**: Supports automatic rehashing based on load factor.

## Implementation Details

The implementation uses a hash table with separate chaining to handle collisions. Each bucket in the hash table is implemented as a `forward_list` to store elements that hash to the same bucket. The container automatically rehashes when the load factor exceeds the maximum load factor (default is 1.0).

Key implementation features:
1. Dynamic resizing based on load factor
2. Separate chaining for collision resolution
3. Customizable hash function and equality comparison
4. Efficient iterator implementation

## Basic Usage Examples

```cpp
#include "unordered_set.h"
#include <iostream>
#include <string>

int main() {
    // Default constructor
    custom::unordered_set<std::string> mySet;

    // Insert elements
    mySet.insert("apple");
    mySet.insert("banana");
    mySet.insert("cherry");
    mySet.insert("banana"); // Duplicate element, will be ignored

    // Check size
    std::cout << "Set size: " << mySet.size() << std::endl; // Output: 3

    // Iterate through elements (order is not guaranteed)
    std::cout << "Set elements: ";
    for (const auto& element : mySet) {
        std::cout << element << " ";
    }
    std::cout << std::endl;

    // Check for element presence
    std::cout << "Contains 'banana'? " << (mySet.contains("banana") ? "Yes" : "No") << std::endl; // Output: Yes
    std::cout << "Contains 'fig'? " << (mySet.contains("fig") ? "Yes" : "No") << std::endl; // Output: No

    // Find an element
    auto it = mySet.find("cherry");
    if (it != mySet.end()) {
        std::cout << "Found element: " << *it << std::endl; // Output: Found element: cherry
    }

    // Erase an element
    size_t erased = mySet.erase("banana");
    std::cout << "Erased " << erased << " element(s)" << std::endl; // Output: Erased 1 element(s)

    // Custom hash function example
    struct StringHash {
        size_t operator()(const std::string& s) const {
            size_t hash = 0;
            for (char c : s) {
                hash = hash * 31 + c;
            }
            return hash;
        }
    };

    custom::unordered_set<std::string, StringHash> customSet;
    customSet.insert("apple");
    customSet.insert("banana");
    customSet.insert("cherry");

    return 0;
}
```

## Implemented Features

*   Constructors:
    *   Default constructor
    *   Constructor with custom hash and equality
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
    *   Forward iteration
*   Capacity:
    *   `empty()`
    *   `size()`
    *   `bucket_count()`
    *   `load_factor()`
    *   `max_load_factor()`
*   Modifiers:
    *   `clear()`
    *   `insert()`
    *   `erase()`
*   Lookup:
    *   `find()`
    *   `count()`
    *   `contains()`

## Future Enhancements

*   Add support for reverse iterators
*   Implement `emplace()` and `emplace_hint()` for in-place construction
*   Add support for custom allocators
*   Implement comparison operators (`==`, `!=`)
*   Add support for range-based operations
*   Add support for node-based operations (extract, merge)
*   Add support for range-based insert and erase operations
*   Implement bucket interface (bucket_size, begin(n), end(n))
*   Add support for rehash with custom bucket count 