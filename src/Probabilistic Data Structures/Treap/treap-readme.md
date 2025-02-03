# Treap Data Structure: Advanced Randomized Binary Search Tree

## Overview

A Treap (Tree + Heap) is an advanced probabilistic data structure that combines the properties of a binary search tree and a heap. It provides efficient average-case performance for various operations while maintaining a simple and elegant implementation.

## Key Features

- **Balanced Tree Operations**: O(log n) time complexity for insert, delete, search
- **Order Statistics**: Efficient k-th element retrieval and order of key operations
- **Randomization**: Built-in randomization ensures probabilistic balance
- **Highly Customizable**: Supports custom comparators and random number generators
- **Modern C++17 Implementation**: Leverages smart pointers and modern language features

## Theoretical Background

### How Treaps Work

A Treap maintains two invariants simultaneously:
1. **Binary Search Tree Property**: Ordered by key
2. **Heap Property**: Ordered by randomly assigned priority

This dual-ordering ensures:
- Efficient searching like a Binary Search Tree
- Probabilistic self-balancing without complex rotations
- Performance close to perfectly balanced trees

### Complexity

| Operation           | Average Case | Worst Case |
|---------------------|--------------|------------|
| Insertion           | O(log n)     | O(n)       |
| Deletion            | O(log n)     | O(n)       |
| Search              | O(log n)     | O(n)       |
| K-th Element        | O(log n)     | O(n)       |
| Order of Key        | O(log n)     | O(n)       |

## Usage Example

```cpp
#include "treap.hpp"

int main() {
    // Create a Treap of integers
    ds::Treap<int> treap;

    // Insert elements
    treap.insert(5);
    treap.insert(3);
    treap.insert(7);

    // Check operations
    assert(treap.contains(5));    // True
    assert(treap.size() == 3);    // Tree size
    assert(treap.order_of_key(5) == 1);  // Order statistic

    // Find k-th smallest element
    auto kth = treap.find_kth(1);  // Returns 5
    
    // Delete element
    treap.erase(5);
}
```

## Advanced Usage: Custom Comparator

```cpp
// Treap with descending order
ds::Treap<int, std::greater<int>> descending_treap;

descending_treap.insert(5);
descending_treap.insert(3);
descending_treap.insert(7);

// K-th element now returns elements in descending order
auto top = descending_treap.find_kth(0);  // Returns 7
```

## Performance Optimizations

1. **Memory Management**
   - Uses `std::unique_ptr` for automatic, efficient memory handling
   - Prevents memory leaks
   - Enables move semantics

2. **Inline Size Tracking**
   - Constant-time size queries
   - Avoids recursive traversals
   - Improves overall performance

3. **Randomization Strategy**
   - Uses `std::mt19937_64` for high-quality random number generation
   - Ensures probabilistic balance
   - Prevents potential worst-case scenarios

## Installation and Integration

1. Copy `treap.hpp` into your project
2. Include the header in your source files
3. Compile with C++17 support (`-std=c++17`)

### Compiler Requirements
- C++17 compliant compiler
- Recommended: GCC 8+, Clang 7+, MSVC 19.20+

### Compilation Example

```bash
g++ -std=c++17 -O3 your_program.cpp -o your_program
```

## Limitations and Considerations

- Not thread-safe
- Probabilistic balancing (not guaranteed perfect balance)
- Slightly higher memory overhead compared to simple BST

## References

1. Aragon, C., & Seidel, R. (1989). Randomized Search Trees.
2. Brodal, G. S. (1996). Worst-case efficient priority queues.

## Performance Tips

- For large datasets, pre-allocate expected number of elements
- Avoid frequent small insertions/deletions
- Use move semantics when possible
