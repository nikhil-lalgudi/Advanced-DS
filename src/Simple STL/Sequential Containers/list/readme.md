# `custom::list`

## Overview

`custom::list` is a sequential container that implements a doubly-linked list. It allows for constant time insertion and deletion of elements at any point within the sequence. Unlike `custom::vector` (or `std::vector`), `custom::list` does not provide random access to elements; access to an element is typically achieved by iterating through the list.

This implementation uses sentinel nodes for the head and tail, which can simplify some boundary conditions in list operations.

## Key Features

*   **Dynamic Size**: The list can grow or shrink as needed.
*   **Constant Time Insertions/Deletions**: Adding or removing elements is O(1) once the position for insertion/deletion is known (i.e., an iterator to the position is available).
*   **Bidirectional Iteration**: Supports iterating forwards and backwards.
*   **No Element Invalidation (Mostly)**: Iterators, pointers, and references to elements are generally not invalidated when other elements are inserted or erased, unless the element itself is erased.

## Disadvantages/Considerations

*   **No Random Access**: Accessing an element by index is O(n) as it requires traversing the list.
*   **Higher Memory Overhead**: Each element requires extra storage for two pointers (previous and next) compared to array-based containers like `custom::vector`.
*   **Cache Inefficiency**: Elements are not stored contiguously in memory, which can lead to poorer cache performance compared to `custom::vector`.

## Basic Usage Examples

```cpp
#include "list.h"
#include <iostream>

int main() {
    // Default constructor
    custom::list<int> myList;

    // Add elements
    myList.push_back(10);    // myList: {10}
    myList.push_front(5);    // myList: {5, 10}
    myList.push_back(20);    // myList: {5, 10, 20}

    std::cout << "Initial list: ";
    for (int val : myList) {
        std::cout << val << " ";
    }
    std::cout << std::endl; // Output: Initial list: 5 10 20 

    // Element access
    std::cout << "Front element: " << myList.front() << std::endl; // Output: 5
    std::cout << "Back element: " << myList.back() << std::endl;   // Output: 20

    // Insert an element
    custom::list<int>::iterator it = myList.begin();
    ++it; // it now points to 10
    myList.insert(it, 7); // myList: {5, 7, 10, 20}
    
    std::cout << "List after inserting 7: ";
    for (int val : myList) {
        std::cout << val << " ";
    }
    std::cout << std::endl; // Output: List after inserting 7: 5 7 10 20

    // Erase an element
    it = myList.begin();
    ++it; // it points to 7
    it = myList.erase(it); // erases 7, it now points to 10
    // myList: {5, 10, 20}

    std::cout << "List after erasing 7 (iterator points to " << *it << "): ";
    for (int val : myList) {
        std::cout << val << " ";
    }
    std::cout << std::endl; // Output: List after erasing 7 (iterator points to 10): 5 10 20 

    // Pop elements
    myList.pop_front(); // myList: {10, 20}
    myList.pop_back();  // myList: {10}

    std::cout << "List after pop_front and pop_back: ";
    for (int val : myList) {
        std::cout << val << " ";
    }
    std::cout << std::endl; // Output: List after pop_front and pop_back: 10 

    // Check size and emptiness
    std::cout << "Size: " << myList.size() << std::endl;       // Output: Size: 1
    std::cout << "Is empty? " << (myList.empty() ? "Yes" : "No") << std::endl; // Output: Is empty? No

    // Clear the list
    myList.clear();
    std::cout << "Size after clear: " << myList.size() << std::endl; // Output: Size after clear: 0
    std::cout << "Is empty after clear? " << (myList.empty() ? "Yes" : "No") << std::endl; // Output: Is empty after clear? Yes

    return 0;
}
```

## Implemented Features

*   Constructors (default, fill, range, copy, move, initializer_list)
*   Destructor
*   Assignment operators (copy, move, initializer_list)
*   Element access (`front`, `back`)
*   Iterators (`begin`, `end`, `rbegin`, `rend`, `cbegin`, `cend`, `crbegin`, `crend`)
*   Capacity (`empty`, `size`)
*   Modifiers:
    *   `clear`
    *   `insert` (single element, fill, range, initializer_list)
    *   `erase` (single element, range)
    *   `push_back`, `pop_back`
    *   `push_front`, `pop_front`
    *   `resize`
    *   `swap`
*   Non-member comparison operators (`==`, `!=`, `<`, `<=`, `>`, `>=`)
*   Non-member `swap`

## Future Enhancements (Potentially)

*   Allocator support (more deeply integrated if necessary beyond default).
*   Operations like `splice`, `remove`, `remove_if`, `unique`, `merge`, `sort`, `reverse`.
*   Full `emplace` and `emplace_front`/`emplace_back` methods with perfect forwarding. 



