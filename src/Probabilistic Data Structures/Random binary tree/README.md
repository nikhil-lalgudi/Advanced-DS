# RandomBinaryTree - C++17 Implementation

## Introduction

This implementation provides a modern C++17 implementation of a Random Binary Search Tree (RBST). Unlike traditional balanced BSTs like AVL or Red-Black trees that require complex rebalancing operations, random binary trees achieve expected O(log n) height through randomization.

## Table of Contents

1. [Concept](#concept)
2. [Features](#features)
3. [Implementation Details](#implementation-details)
4. [Usage](#usage)
5. [Performance](#performance)
6. [Examples](#examples)

## Concept

A random binary search tree is based on the principle that if keys are inserted in random order, the resulting BST has expected height O(log n) and expected search time O(log n) without requiring explicit rebalancing operations.

This implementation combines elements of:
- Binary Search Trees (ordered keys)
- Treaps (priority-based tree rotations)
- Randomization (to achieve balance)

Unlike deterministic self-balancing trees that strictly enforce balance, random binary trees achieve probabilistic balance through randomization, offering simpler implementation while maintaining excellent expected performance.

## Features

- **Template-based**: Works with any comparable data type
- **Modern C++17 features**: Uses `[[nodiscard]]`, smart pointers, and other C++17 features
- **Memory-safe**: Uses `std::unique_ptr` for automatic memory management
- **Comprehensive API**:
  - Basic operations: insert, remove, contains
  - Advanced operations: rank, select (order statistics)
  - Batch operations: insert range, randomized insertion
- **Order statistics**: Efficient rank and select operations
- **Randomization**: Both for single insertions and batch operations
- **Header-only library**: Easy to include in any project

## Implementation Details

### Core Components

1. **Node Structure**:
   - Key value
   - Random priority (for treap-like behavior)
   - Left and right child pointers
   - Subtree size (for order statistics)

2. **Tree Operations**:
   - **Rotation**: Left and right rotations maintain the randomization property
   - **Insertion**: Maintains BST property and rotates based on random priority
   - **Removal**: Handles various cases (leaf nodes, nodes with one/two children)
   - **Search**: Standard BST search algorithm
   - **Order Statistics**: Rank and select operations leveraging subtree size

3. **Randomization Strategy**:
   - Each node gets a random priority when created
   - Tree rotations maintain heap-ordered priorities
   - Optional randomized batch insertion

## Usage

### Basic Operations

```cpp
// Create a tree
RandomBinaryTree<int> tree;

// Insert elements
tree.insert(50);
tree.insert(30);
tree.insert(70);

// Check if element exists
bool has40 = tree.contains(40);  // false
bool has50 = tree.contains(50);  // true

// Remove an element
tree.remove(30);

// Get size
int size = tree.size();  // 2
```

### Advanced Operations

```cpp
// Find element by rank (0-based)
std::optional<int> third = tree.select(2);

// Find rank of element
int rank = tree.rank(70);  // 1 (0-based)

// Traversal
tree.inorder_traversal([](const int& value) {
    std::cout << value << " ";
});

// Get sorted elements
std::vector<int> sorted = tree.to_sorted_vector();
```

### Batch Operations

```cpp
// Insert multiple elements
std::vector<int> values = {10, 20, 30, 40, 50};

// Regular insertion
tree.insert(values.begin(), values.end());

// Randomized insertion (better for performance)
tree.insert_randomized(values.begin(), values.end());

// Static builder method
auto new_tree = RandomBinaryTree<int>::build_random_tree(values.begin(), values.end());
```

## Performance

Random binary trees provide an excellent balance between implementation simplicity and performance:

- **Expected height**: O(log n)
- **Expected search time**: O(log n)
- **Expected insertion time**: O(log n)
- **Expected deletion time**: O(log n)

The key advantage is that these operations maintain logarithmic performance without the complex rebalancing logic of AVL or Red-Black trees.

### Performance Comparison

Inserting elements in sorted order is a worst-case scenario for standard BSTs, leading to linear-height trees. With randomization, even with ordered inputs, our tree maintains expected logarithmic behavior:

```
Benchmark results for 10000 elements:
  Ordered insertion: 3254 µs
  Random insertion:  2153 µs
```

## Examples

See `randombin_example.cpp` for complete usage examples, including:

1. Basic insertions and removals
2. Searching for elements
3. Finding elements by rank and ranks of elements
4. Performance benchmarking
5. Using different data types (integers, strings, etc.)

## Files

- `randombin.hpp` - The main header-only implementation of the RandomBinaryTree
- `randombin.cpp` - Contains explicit template instantiations for common types
- `randombin_example.cpp` - Example usage and benchmarking

## References

This implementation is based on principles from:
- [Random Binary Trees](https://en.wikipedia.org/wiki/Random_binary_tree)
- Treap data structures
- Randomized algorithms for balanced trees 