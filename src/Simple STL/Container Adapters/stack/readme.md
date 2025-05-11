# `custom::stack`

## Overview

`custom::stack` is a container adapter that provides a Last-In, First-Out (LIFO) data structure. It is implemented as an adapter over an underlying container, which defaults to `custom::vector<T>`. Elements are added (pushed) and removed (popped) from the same end, referred to as the "top" of the stack.

## Key Features

*   **LIFO Operations**: Strictly follows the Last-In, First-Out principle.
*   **Adapts Existing Containers**: Can use different underlying sequential containers (like `custom::vector`, `custom::list`, or a custom `deque`) that support the required operations (`empty`, `size`, `back`, `push_back`, `pop_back`).
*   **Simple Interface**: Provides a restricted interface focused on stack operations: `push`, `pop`, `top`, `empty`, `size`.

## Underlying Container Requirements

The underlying container `Container` must provide the following operations:

*   `empty()`: Returns `true` if the container is empty.
*   `size()`: Returns the number of elements.
*   `back()`: Accesses the last element (the top of the stack).
*   `push_back(const value_type&)` and `push_back(value_type&&)`: Adds an element to the end (pushes onto the stack).
*   `pop_back()`: Removes the last element (pops from the stack).
*   (Optionally for `emplace`): `emplace_back(Args&&... args)` or a way to construct in place at the back. The current simplified `emplace` falls back to `push_back(T(...))`.

Both `custom::vector` (the default) and `custom::list` satisfy these requirements.

## Basic Usage Examples

```cpp
#include "stack.h"
#include "../Sequential Containers/list/list.h" // For custom::list example
#include <iostream>
#include <string>

int main() {
    // Default stack (uses custom::vector<int>)
    custom::stack<int> myStack;

    // Check if empty
    std::cout << "Is stack empty? " << (myStack.empty() ? "Yes" : "No") << std::endl; // Output: Yes

    // Push elements
    myStack.push(10);
    myStack.push(20);
    myStack.push(30);
    // Stack: top -> 30, 20, 10

    std::cout << "Stack size: " << myStack.size() << std::endl; // Output: 3
    std::cout << "Top element: " << myStack.top() << std::endl;   // Output: 30

    // Pop an element
    myStack.pop(); // Removes 30
    // Stack: top -> 20, 10
    std::cout << "After pop, top element: " << myStack.top() << std::endl; // Output: 20
    std::cout << "Stack size: " << myStack.size() << std::endl; // Output: 2

    // Modify top element (if underlying container allows mutable access through back())
    if (!myStack.empty()) {
        myStack.top() = 25; // Modifies 20 to 25
    }
    // Stack: top -> 25, 10
    std::cout << "After modification, top: " << myStack.top() << std::endl;

    // Emplace an element (simplified version)
    myStack.emplace(40); // Effectively push_back(T(40))
    // Stack: top -> 40, 25, 10
    std::cout << "After emplace(40), top element: " << myStack.top() << std::endl; // Output: 40
    std::cout << "Stack size: " << myStack.size() << std::endl; // Output: 3

    // Pop remaining elements
    std::cout << "Popping all elements from vector-based stack:" << std::endl;
    while(!myStack.empty()) {
        std::cout << "Popping: " << myStack.top() << std::endl;
        myStack.pop();
    }
    std::cout << "Is stack empty now? " << (myStack.empty() ? "Yes" : "No") << std::endl; // Output: Yes

    // Stack with custom::list as underlying container
    custom::stack<std::string, custom::list<std::string>> listStack;
    listStack.push("hello");
    listStack.push("world");
    std::cout << "listStack top: " << listStack.top() << std::endl; // Output: world
    listStack.pop();
    std::cout << "listStack top after pop: " << listStack.top() << std::endl; // Output: hello

    return 0;
}
```

## Implemented Features

*   Constructors (default, from container copy, from container move).
*   Copy and move semantics (defaulted, relying on the underlying container).
*   Element access (`top`).
*   Capacity (`empty`, `size`).
*   Modifiers:
    *   `push` (lvalue, rvalue).
    *   `emplace` (simplified, constructs then `push_back`).
    *   `pop`.
    *   `swap`.
*   Non-member comparison operators (`==`, `!=`, `<`, `<=`, `>`, `>=`) based on the underlying container's operators.
*   Non-member `swap`.

## Considerations

*   The efficiency of `custom::stack` operations depends on the chosen underlying container. Both `custom::vector` and `custom::list` provide O(1) (amortized for `vector::push_back` if reallocation occurs) for all core stack operations.
*   The `emplace` method is currently a simplified version. A full-featured `emplace` would ideally construct the element in-place at the back (top) of the underlying container using `emplace_back` if available. 