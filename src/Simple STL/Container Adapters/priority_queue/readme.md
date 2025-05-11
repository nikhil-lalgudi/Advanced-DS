# `custom::priority_queue`

## Overview

`custom::priority_queue` is a container adapter that provides a priority queue data structure. Elements are ordered according to a comparison function, and the element with the highest priority (based on this comparison) is always at the "top" of the queue. By default, it uses `custom::vector<T>` as the underlying container and `std::less<T>` as the comparator, resulting in a max-heap (largest element has highest priority).

It uses standard library heap algorithms (`std::make_heap`, `std::push_heap`, `std::pop_heap`) to maintain the heap property within the underlying container.

## Key Features

*   **Priority-Based Retrieval**: Allows efficient retrieval of the element with the highest priority (O(1) for `top()`).
*   **Logarithmic Insertions/Deletions**: Adding (`push`, `emplace`) and removing (`pop`) elements takes logarithmic time (O(log n)) due to heap maintenance.
*   **Flexible Underlying Container and Comparator**: Can be configured with different underlying sequence containers (that support random access iterators, `front()`, `push_back()`, `pop_back()`) and custom comparison functions.

## Underlying Container Requirements

The underlying container `Container` must provide:

*   Random access iterators (`begin()`, `end()`).
*   `front()`: To access the top element (highest priority).
*   `push_back(const value_type&)` and `push_back(value_type&&)`: To add elements before heapifying.
*   `pop_back()`: To remove the element after `std::pop_heap` moves it to the end.
*   `empty()`, `size()`.

`custom::vector` (the default) fulfills these requirements. `custom::list` would not be suitable due to lack of random access iterators required by heap algorithms.

## Comparator

*   The `Compare` function object determines the priority. `std::less<T>` (default) creates a max-heap where `comp(a, b)` is true if `a` is considered "less than" `b`. Heap algorithms arrange elements such that the element for which `comp` is false against all others (or true for fewer others) comes to the front. For `std::less`, this means the largest element is at the front.
*   To create a min-heap (smallest element has highest priority), `std::greater<T>` should be used as the `Compare` type.

## Basic Usage Examples

```cpp
#include "priority_queue.h"
#include <iostream>
#include <string>
#include <vector>     // For initializing data
#include <functional> // For std::greater

int main() {
    // Default priority_queue (max-heap with custom::vector<int>)
    custom::priority_queue<int> maxHeap;

    maxHeap.push(30);
    maxHeap.push(100);
    maxHeap.push(20);
    // Heap: top is 100

    std::cout << "Max-heap top: " << maxHeap.top() << std::endl; // Output: 100
    std::cout << "Max-heap size: " << maxHeap.size() << std::endl; // Output: 3

    maxHeap.emplace(50); // Simplified emplace
    // Heap: top is 100
    std::cout << "After emplace(50), top: " << maxHeap.top() << std::endl; // Output: 100

    std::cout << "Popping from max-heap:" << std::endl;
    while(!maxHeap.empty()) {
        std::cout << maxHeap.top() << " ";
        maxHeap.pop();
    }
    std::cout << std::endl; // Output: 100 50 30 20 (or other valid heap order during pop)

    // Min-heap (using std::greater<int>)
    custom::priority_queue<int, custom::vector<int>, std::greater<int>> minHeap;
    minHeap.push(30);
    minHeap.push(100);
    minHeap.push(20);
    minHeap.push(5);
    // Heap: top is 5

    std::cout << "Min-heap top: " << minHeap.top() << std::endl; // Output: 5

    std::cout << "Popping from min-heap:" << std::endl;
    while(!minHeap.empty()) {
        std::cout << minHeap.top() << " ";
        minHeap.pop();
    }
    std::cout << std::endl; // Output: 5 20 30 100 (or other valid heap order during pop)

    // Constructor with iterators
    std::vector<int> data = {40, 10, 70, 25};
    custom::priority_queue<int> pqFromIter(data.begin(), data.end());
    std::cout << "PQ from iterators, top: " << pqFromIter.top() << std::endl; // Output: 70

    return 0;
}
```

## Implemented Features

*   Constructors:
    *   Default.
    *   With comparator.
    *   With comparator and container copy/move.
    *   Range constructor (with comparator, optional container copy/move).
*   Copy and move semantics (defaulted, relying on underlying container and comparator).
*   Element access (`top` - const reference).
*   Capacity (`empty`, `size`).
*   Modifiers:
    *   `push` (lvalue, rvalue).
    *   `emplace` (simplified, constructs then `push_back` and heapifies).
    *   `pop`.
    *   `swap` (swaps container and comparator).
*   Non-member `swap`.

## Considerations

*   The `emplace` method is a simplified version. A full-featured `emplace` would construct the element in-place in the underlying container before heapifying.
*   This implementation does not provide direct iterators to traverse the heap, as `std::priority_queue` typically does not.
*   Relies on the correctness and efficiency of the standard library heap algorithms. 