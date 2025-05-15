# Unordered Multimap

## Overview
`custom::unordered_multimap` is an associative container that stores key-value pairs in an unordered manner, allowing multiple elements with the same key. It is implemented using a hash table with separate chaining to handle collisions.

## Key Features
- Multiple elements with the same key
- Unordered storage based on hash values
- Efficient operations:
  - Average case: O(1) for insertions, deletions, and lookups
  - Worst case: O(n) for insertions, deletions, and lookups
- Custom hash functions
- Custom key equality
- Load factor control

## Implementation Details
The implementation uses a hash table with separate chaining to handle collisions. Each bucket in the hash table is implemented as a forward list, allowing efficient insertions and deletions. The container automatically resizes when the load factor exceeds the maximum load factor.

## Basic Usage Examples

```cpp
#include "unordered_multimap.h"
#include <string>
#include <iostream>

int main() {
    // Create an unordered_multimap with string keys and int values
    custom::unordered_multimap<std::string, int> map;

    // Insert elements
    map.insert({"apple", 1});
    map.insert({"banana", 2});
    map.insert({"apple", 3}); // Multiple values for the same key

    // Iterate over elements
    for (const auto& pair : map) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    // Find elements
    auto it = map.find("apple");
    if (it != map.end()) {
        std::cout << "Found apple with value: " << it->second << std::endl;
    }

    // Count elements with a specific key
    size_t count = map.count("apple");
    std::cout << "Number of apples: " << count << std::endl;

    // Custom hash function
    struct StringHash {
        size_t operator()(const std::string& s) const {
            size_t hash = 0;
            for (char c : s) {
                hash = hash * 31 + c;
            }
            return hash;
        }
    };

    custom::unordered_multimap<std::string, int, StringHash> custom_map;
    // ... use custom_map as needed
}
```

## Implemented Features

### Constructors and Destructors
- Default constructor
- Initializer list constructor
- Copy constructor
- Move constructor
- Destructor
- Assignment operators (copy, move, initializer list)

### Iterators
- Forward iterators
- begin() and end() methods

### Capacity
- empty()
- size()
- bucket_count()
- load_factor()
- max_load_factor()

### Modifiers
- clear()
- insert()
- erase()

### Lookup
- find()
- count()
- contains()

## Future Enhancements
- Reverse iterators
- In-place construction
- Custom allocators
- Comparison operators
- Range-based operations
- Bucket interface
- Hash policy
- Exception safety improvements
- Thread safety
- Performance optimizations 