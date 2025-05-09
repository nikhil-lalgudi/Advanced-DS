# Forward List

A singly linked list implementation similar to C++ STL's `std::forward_list`.

## Overview

The `custom::forward_list<T>` is a container that supports fast insertion and removal of elements from anywhere in the container. Fast random access is not supported. It is implemented as a singly-linked list, meaning that each node contains a value and a pointer to the next node. The name "forward_list" comes from the fact that iteration can only proceed "forward" through the list, not backwards.

## Advantages

- Constant time insertions and deletions at any position (given an iterator to that position)
- No reallocation when adding elements
- Iterators remain valid even when other elements are added (unlike vector)

## Disadvantages

- No direct random access (need to traverse from beginning)
- More memory overhead per element (due to storing the next pointer)
- No backward traversal (unlike `std::list`)
- Cannot directly access the last element

## Core Operations

### Constructors

```cpp
// Default constructor
forward_list();

// Fill constructor
forward_list(size_type count, const T& value);

// Count constructor (with default values)
explicit forward_list(size_type count);

// Range constructor
template <typename InputIt>
forward_list(InputIt first, InputIt last);

// Copy constructor
forward_list(const forward_list& other);

// Move constructor
forward_list(forward_list&& other);

// Initializer list constructor
forward_list(std::initializer_list<T> init);
```

### Element Access

```cpp
// Access the first element
reference front();
const_reference front() const;
```

### Iterators

```cpp
// Returns an iterator to the element before the first element
iterator before_begin() noexcept;
const_iterator before_begin() const noexcept;

// Returns an iterator to the first element
iterator begin() noexcept;
const_iterator begin() const noexcept;
const_iterator cbegin() const noexcept;

// Returns an iterator to the element following the last element
iterator end() noexcept;
const_iterator end() const noexcept;
const_iterator cend() const noexcept;
```

### Capacity

```cpp
// Checks if the container is empty
bool empty() const noexcept;

// Returns the number of elements
size_type size() const noexcept;
```

### Modifiers

```cpp
// Clears the contents
void clear() noexcept;

// Inserts elements after the specified position
iterator insert_after(const_iterator pos, const T& value);
iterator insert_after(const_iterator pos, T&& value);

// Erases elements
iterator erase_after(const_iterator pos);
iterator erase_after(const_iterator first, const_iterator last);

// Adds an element to the beginning
void push_front(const T& value);
void push_front(T&& value);

// Removes the first element
void pop_front();

// Changes the number of elements stored
void resize(size_type count, const value_type& value = value_type());

// Swaps the contents
void swap(forward_list& other) noexcept;
```

### Operations

```cpp
// Merges two sorted lists
void merge(forward_list& other);

// Moves elements from another forward_list
void splice_after(const_iterator pos, forward_list& other);

// Removes all elements satisfying specific criteria
size_type remove(const T& value);
template <typename UnaryPredicate>
size_type remove_if(UnaryPredicate p);

// Reverses the order of elements
void reverse() noexcept;

// Sorts the elements
void sort();
```

## Example Usage

```cpp
#include "forwardlist.h"
#include <iostream>
#include <string>

int main() {
    // Create a forward_list of integers
    custom::forward_list<int> numbers{1, 2, 3, 4, 5};
    
    // Add elements to the front
    numbers.push_front(0);
    
    // Insert after the first element
    auto it = numbers.begin();
    numbers.insert_after(it, 10);
    
    // Iterate and print
    std::cout << "Numbers: ";
    for (const auto& num : numbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    
    // Remove even numbers
    numbers.remove_if([](int n) { return n % 2 == 0; });
    
    // Sort the list
    numbers.sort();
    
    // Print after modifications
    std::cout << "After removal and sort: ";
    for (const auto& num : numbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    
    return 0;
}
```

## Implementation Notes

This implementation is a simplified version of the C++ STL `std::forward_list` with the following characteristics:

- Uses raw pointers instead of allocators for memory management
- Provides core functionality while maintaining a lightweight codebase
- Lacks some advanced features of the STL implementation (like specialized allocator support)
- Focuses on the fundamental operations most commonly used
