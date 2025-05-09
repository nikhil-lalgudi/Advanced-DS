#include "randombin.hpp"
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <chrono>
#include <algorithm>

// Helper function to print a tree structure
void print_tree_structure(const std::vector<int>& values) {
    std::cout << "Tree elements in sorted order: ";
    for (const auto& val : values) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
}

// Benchmark function to compare random vs ordered insertion
void benchmark_insertion(int num_elements) {
    std::vector<int> values(num_elements);
    
    // Generate ordered values
    for (int i = 0; i < num_elements; ++i) {
        values[i] = i;
    }
    
    // Benchmark ordered insertion
    auto start = std::chrono::high_resolution_clock::now();
    RandomBinaryTree<int> ordered_tree;
    for (int val : values) {
        ordered_tree.insert(val);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto ordered_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    
    // Benchmark randomized insertion
    start = std::chrono::high_resolution_clock::now();
    RandomBinaryTree<int> random_tree;
    random_tree.insert_randomized(values.begin(), values.end());
    end = std::chrono::high_resolution_clock::now();
    auto random_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    
    std::cout << "Benchmark results for " << num_elements << " elements:" << std::endl;
    std::cout << "  Ordered insertion: " << ordered_duration << " µs" << std::endl;
    std::cout << "  Random insertion:  " << random_duration << " µs" << std::endl;
}

int main() {
    std::cout << "Random Binary Tree Example" << std::endl;
    std::cout << "==========================" << std::endl;
    
    // Create a random binary tree with integers
    RandomBinaryTree<int> tree;
    
    // Insert some values
    std::cout << "\nInserting elements: 50, 30, 70, 20, 40, 60, 80" << std::endl;
    tree.insert(50);
    tree.insert(30);
    tree.insert(70);
    tree.insert(20);
    tree.insert(40);
    tree.insert(60);
    tree.insert(80);
    
    // Print the tree in sorted order
    auto values = tree.to_sorted_vector();
    print_tree_structure(values);
    
    // Find elements
    std::cout << "\nSearching for elements:" << std::endl;
    std::cout << "Contains 40: " << std::boolalpha << tree.contains(40) << std::endl;
    std::cout << "Contains 55: " << tree.contains(55) << std::endl;
    
    // Find elements by rank
    std::cout << "\nFinding elements by rank:" << std::endl;
    for (int i = 0; i < tree.size(); ++i) {
        auto element = tree.select(i);
        if (element) {
            std::cout << "Element at rank " << i << ": " << *element << std::endl;
        }
    }
    
    // Find ranks of elements
    std::cout << "\nFinding ranks of elements:" << std::endl;
    for (int elem : {20, 40, 60, 90}) {
        int rank = tree.rank(elem);
        if (rank != -1) {
            std::cout << "Rank of " << elem << ": " << rank << std::endl;
        } else {
            std::cout << "Element " << elem << " not found" << std::endl;
        }
    }
    
    // Remove an element
    std::cout << "\nRemoving element 30" << std::endl;
    tree.remove(30);
    values = tree.to_sorted_vector();
    print_tree_structure(values);
    
    // Creating a tree with randomized insertions
    std::cout << "\nCreating a tree with randomized insertions:" << std::endl;
    std::vector<int> random_values = {15, 25, 35, 45, 55, 65, 75, 85, 95};
    auto random_tree = RandomBinaryTree<int>::build_random_tree(
        random_values.begin(), random_values.end()
    );
    auto random_tree_values = random_tree.to_sorted_vector();
    print_tree_structure(random_tree_values);
    
    // Run benchmarks
    std::cout << "\nRunning benchmarks:" << std::endl;
    benchmark_insertion(1000);
    benchmark_insertion(10000);
    
    // Tree with string values
    std::cout << "\nTree with string values:" << std::endl;
    RandomBinaryTree<std::string> string_tree;
    string_tree.insert("apple");
    string_tree.insert("banana");
    string_tree.insert("cherry");
    string_tree.insert("date");
    string_tree.insert("elderberry");
    
    string_tree.inorder_traversal([](const std::string& str) {
        std::cout << str << " ";
    });
    std::cout << std::endl;
    
    return 0;
} 