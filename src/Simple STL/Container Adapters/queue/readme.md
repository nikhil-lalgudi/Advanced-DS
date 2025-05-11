# `custom::queue`

## Overview

`custom::queue` is a container adapter that provides a First-In, First-Out (FIFO) data structure. It is implemented as an adapter over an underlying container, which defaults to `custom::list<T>`. Elements are added to the "back" (enqueued) and removed from the "front" (dequeued).

## Key Features

*   **FIFO Operations**: Strictly follows the First-In, First-Out principle.
*   **Adapts Existing Containers**: Can use different underlying sequential containers (like `custom::list` or a custom `deque` if available) that support the required operations (`empty`, `size`, `front`, `back`, `push_back`, `pop_front`).
*   **Simple Interface**: Provides a restricted interface focused on queue operations: `push`, `pop`, `front`, `back`, `empty`, `size`.

## Underlying Container Requirements

The underlying container `Container` must provide the following operations:

*   `empty()`: Returns `true` if the container is empty.
*   `size()`: Returns the number of elements.
*   `front()`: Accesses the first element.
*   `back()`: Accesses the last element.
*   `push_back(const value_type&)` and `push_back(value_type&&)`: Adds an element to the end.
*   `pop_front()`: Removes the first element.
*   (Optionally for `emplace`): `emplace_back(Args&&... args)` or a way to construct in place at the back. The current simplified `emplace` falls back to `push_back(T(...))`.

`custom::list` satisfies these requirements. `custom::vector` would require a `pop_front` operation to be a fully compliant underlying container for `custom::queue` (which it currently lacks, making `pop` inefficient or impossible if directly mapped).

## Basic Usage Examples

```cpp
#include "queue.h"
#include <iostream>
#include <string>

int main() {
    // Default queue (uses custom::list<int>)
    custom::queue<int> myQueue;

    // Check if empty
    std::cout << "Is queue empty? " << (myQueue.empty() ? "Yes" : "No") << std::endl; // Output: Yes

    // Push elements
    myQueue.push(10);
    myQueue.push(20);
    myQueue.push(30);
    // Queue: front -> 10, 20, 30 <- back

    std::cout << "Queue size: " << myQueue.size() << std::endl; // Output: 3
    std::cout << "Front element: " << myQueue.front() << std::endl; // Output: 10
    std::cout << "Back element: " << myQueue.back() << std::endl;   // Output: 30

    // Pop an element
    myQueue.pop(); // Removes 10
    // Queue: front -> 20, 30 <- back
    std::cout << "After pop, front element: " << myQueue.front() << std::endl; // Output: 20
    std::cout << "Queue size: " << myQueue.size() << std::endl; // Output: 2

    // Modify front/back (if underlying container allows mutable access)
    if (!myQueue.empty()) {
        myQueue.front() = 25; // Modifies 20 to 25
        myQueue.back() = 35;  // Modifies 30 to 35
    }
    // Queue: front -> 25, 35 <- back
    std::cout << "After modification, front: " << myQueue.front() << ", back: " << myQueue.back() << std::endl;

    // Emplace an element (simplified version)
    myQueue.emplace(40); // Effectively push_back(T(40))
    // Queue: front -> 25, 35, 40 <- back
    std::cout << "After emplace(40), back element: " << myQueue.back() << std::endl; // Output: 40
    std::cout << "Queue size: " << myQueue.size() << std::endl; // Output: 3

    // Pop remaining elements
    while(!myQueue.empty()) {
        std::cout << "Popping: " << myQueue.front() << std::endl;
        myQueue.pop();
    }
    std::cout << "Is queue empty now? " << (myQueue.empty() ? "Yes" : "No") << std::endl; // Output: Yes

    return 0;
}
```

## Implemented Features

*   Constructors (default, from container copy, from container move).
*   Copy and move semantics (defaulted, relying on the underlying container).
*   Element access (`front`, `back`).
*   Capacity (`empty`, `size`).
*   Modifiers:
    *   `push` (lvalue, rvalue).
    *   `emplace` (simplified, constructs then `push_back`).
    *   `pop`.
    *   `swap`.
*   Non-member comparison operators (`==`, `!=`, `<`, `<=`, `>`, `>=`) based on the underlying container's operators.
*   Non-member `swap`.

## Considerations

*   The efficiency of `custom::queue` operations heavily depends on the chosen underlying container. `custom::list` provides O(1) for all core queue operations.
*   The `emplace` method is currently a simplified version. A full-featured `emplace` would ideally construct the element in-place at the back of the underlying container using `emplace_back` if available on the container. 