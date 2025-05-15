# `custom::unordered_map`

## Overview

`custom::unordered_map` is an associative container that stores key-value pairs in an unordered manner. It is implemented using a hash table with separate chaining, which provides efficient operations for insertion, deletion, and lookup. The elements are not kept in any particular order, but instead are organized based on their hash values.

## Key Features

*   **Unique Keys**: Each key in the map must be unique.
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
*   **Custom Hash Function**: Allows custom hash functions for key types.
*   **Custom Key Equality**: Allows custom equality comparison for keys.
*   **Load Factor Control**: Supports automatic rehashing based on load factor.

## Implementation Details

The implementation uses a hash table with separate chaining to handle collisions. Each bucket in the hash table is implemented as a `forward_list` to store elements that hash to the same bucket. The container automatically rehashes when the load factor exceeds the maximum load factor (default is 1.0).

Key implementation features:
1. Dynamic resizing based on load factor
2. Separate chaining for collision resolution
3. Customizable hash function and key equality
4. Efficient iterator implementation

## Basic Usage Examples

```cpp
#include "unordered_map.h"
#include <iostream>
#include <string>

int main() {
    // Default constructor
    custom::unordered_map<std::string, int> myMap;

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

    // Iterate through elements (order is not guaranteed)
    std::cout << "Map elements: ";
    for (const auto& [key, value] : myMap) {
        std::cout << key << ":" << value << " ";
    }
    std::cout << std::endl;

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

    custom::unordered_map<std::string, int, StringHash> customMap;
    customMap.insert({"apple", 1});
    customMap.insert({"banana", 2});
    customMap.insert({"cherry", 3});

    return 0;
}
```

## Implemented Features

*   Constructors:
    *   Default constructor
    *   Constructor with custom hash and key equality
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

*   Add support for reverse iterators
*   Implement `emplace()` and `emplace_hint()` for in-place construction
*   Add support for custom allocators
*   Implement comparison operators (`==`, `!=`)
*   Add support for range-based operations
*   Add support for node-based operations (extract, merge)
*   Add support for range-based insert and erase operations
*   Implement bucket interface (bucket_size, begin(n), end(n))
*   Add support for rehash with custom bucket count 