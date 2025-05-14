# `custom::unordered_set`

## Overview

`custom::unordered_set` is a container that stores unique elements in no particular order. It uses a hash table to organize elements, allowing for average constant-time complexity for search, insertion, and deletion operations.

This implementation uses a `custom::vector` of `custom::forward_list` buckets (separate chaining) for collision resolution. It provides basic hash policy management (`load_factor`, `max_load_factor`, `rehash`, `reserve`).

## Key Features

*   **Unique Elements**: Automatically handles uniqueness; duplicate insertions are ignored.
*   **Average O(1) Complexity**: Search (`find`, `count`, `contains`), insertion (`insert`), and deletion (`erase` by key) have an average time complexity of O(1). In the worst case (many collisions), these can degrade to O(n).
*   **Hash-Based**: Element organization and retrieval depend on hash functions and equality predicates.
*   **Dynamic Rehashing**: The hash table can automatically resize (rehash) when the load factor exceeds a specified maximum, to maintain performance.

## Disadvantages/Considerations

*   **No Ordering**: Elements are not stored in any specific order. Iteration order is not guaranteed and may change upon rehashing.
*   **Worst-Case O(n)**: Performance can degrade if the hash function is poor or if many elements hash to the same bucket.
*   **Iterator Invalidation**: Rehashing invalidates all iterators. Insertions may invalidate iterators if they cause a rehash. Erasing an element invalidates iterators to that element.
*   **Simplified Iterators**: The provided iterators are simplified (forward iterators) and may not offer all the features or robustness of `std::unordered_set` iterators, especially regarding `erase(iterator)`.

## Basic Usage Examples

```cpp
#include "unordered_set.h"
#include <iostream>
#include <string>
#include <vector>

// Example custom hash and equality for a struct (if needed)
struct MyKey {
    int id;
    std::string name;
    bool operator==(const MyKey& other) const {
        return id == other.id && name == other.name;
    }
};

struct MyKeyHash {
    std::size_t operator()(const MyKey& k) const {
        return std::hash<int>()(k.id) ^ (std::hash<std::string>()(k.name) << 1);
    }
};

int main() {
    // Unordered set of integers (uses std::hash<int> by default)
    custom::unordered_set<int> mySet;

    // Insert elements
    mySet.insert(10);
    mySet.insert(20);
    mySet.insert(5);
    mySet.insert(20); // Duplicate, will be ignored

    std::cout << "Set elements: ";
    for (int val : mySet) {
        std::cout << val << " "; // Order is not guaranteed
    }
    std::cout << "(Size: " << mySet.size() << ")" << std::endl;
    // Example Output: Set elements: 5 10 20 (Size: 3) or 10 5 20 (Size: 3) etc.

    // Check for element presence
    std::cout << "Contains 10? " << (mySet.contains(10) ? "Yes" : "No") << std::endl; // Output: Yes
    std::cout << "Contains 15? " << (mySet.contains(15) ? "Yes" : "No") << std::endl; // Output: No
    std::cout << "Count of 20: " << mySet.count(20) << std::endl; // Output: 1

    // Find an element
    custom::unordered_set<int>::iterator it = mySet.find(5);
    if (it != mySet.end()) {
        std::cout << "Found element: " << *it << std::endl; // Output: Found element: 5
    } else {
        std::cout << "Element 5 not found." << std::endl;
    }

    // Erase an element by key
    size_t erased_count = mySet.erase(10);
    std::cout << "Erased " << erased_count << " element(s) with key 10." << std::endl; // Output: 1
    std::cout << "Contains 10 after erase? " << (mySet.contains(10) ? "Yes" : "No") << std::endl; // Output: No

    std::cout << "Set after erasing 10: ";
    for (int val : mySet) {
        std::cout << val << " ";
    }
    std::cout << "(Size: " << mySet.size() << ")" << std::endl;

    // Hash policy
    std::cout << "Initial bucket count: " << mySet.bucket_count() << std::endl;
    std::cout << "Initial load factor: " << mySet.load_factor() << std::endl;
    mySet.max_load_factor(0.75f);
    mySet.insert(30); mySet.insert(40); mySet.insert(50); // May trigger rehash
    std::cout << "After inserts & potential rehash: " << std::endl;
    std::cout << "  Bucket count: " << mySet.bucket_count() << std::endl;
    std::cout << "  Load factor: " << mySet.load_factor() << std::endl;

    mySet.clear();
    std::cout << "Is set empty after clear? " << (mySet.empty() ? "Yes" : "No") << std::endl;

    // Example with custom types
    custom::unordered_set<MyKey, MyKeyHash> customSet;
    customSet.insert({1, "Alice"});
    customSet.insert({2, "Bob"});
    if (customSet.contains({1, "Alice"})) {
        std::cout << "Custom set contains {1, \"Alice\"}" << std::endl;
    }

    return 0;
}
```

## Implemented Features

*   Underlying storage: `custom::vector` of `custom::forward_list` (chaining).
*   Constructors (default, bucket_count, range, copy, move, initializer_list) with support for custom Hash and KeyEqual.
*   Destructor (defaulted).
*   Assignment operators (copy, move, initializer_list).
*   Iterators (simplified forward iterator: `begin`, `end`, `cbegin`, `cend`).
*   Capacity (`empty`, `size`).
*   Modifiers:
    *   `clear`
    *   `insert` (single value, rvalue, range, initializer_list) - returns `std::pair<iterator, bool>`.
    *   `erase` (by key) - returns count of erased elements.
    *   `swap`.
*   Lookup:
    *   `count` (returns 0 or 1).
    *   `find` (returns iterator or `end()`).
    *   `contains` (convenience wrapper around `find`).
*   Bucket interface (`bucket_count`).
*   Hash policy:
    *   `load_factor`
    *   `max_load_factor` (get and set)
    *   `rehash`
    *   `reserve`
*   Observers (`hash_function`, `key_eq`).
*   Non-member `swap`.

## Simplifications & Future Enhancements

*   **Iterators**: The current iterators are simplified. A full implementation would require more complex iterators to correctly handle skipping empty buckets and supporting operations like `erase(iterator)` robustly. The `insert` and `find` methods return iterators, but their construction is simplified.
*   **`emplace` Methods**: `emplace` and `emplace_hint` are not yet implemented.
*   **`erase(iterator)`**: Erasing elements using an iterator is complex to implement correctly with the current iterator design and is omitted.
*   **Allocator Support**: While the `BucketVector` uses an allocator, per-element or per-node allocator support (common in STL containers) is more involved and not fully implemented for the `forward_list` nodes within this context.
*   **Equality Operator**: `operator==` for comparing two `unordered_set`s (element-wise, order-independent) is not provided.
*   **Advanced Bucket Interface**: Functions like `bucket_size`, `local_iterator`, `c_local_iterator` are not implemented.
``` 