#include "deque.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <numeric>

// Helper function for testing
void print_line(const std::string& test_name) {
    std::cout << "\n=== " << test_name << " ===" << std::endl;
}

// Test basic functionality
void test_basic() {
    print_line("Basic Tests");
    
    // Test creation and element access
    custom::deque<int> dq;
    dq.push_back(10);
    dq.push_back(20);
    dq.push_back(30);
    dq.push_front(5);
    dq.push_front(1);
    
    std::cout << "Deque elements: ";
    for (size_t i = 0; i < dq.size(); ++i) {
        std::cout << dq[i] << " ";
    }
    std::cout << std::endl;
    
    // Test element access methods
    std::cout << "dq[2]: " << dq[2] << std::endl;
    std::cout << "dq.at(3): " << dq.at(3) << std::endl;
    std::cout << "dq.front(): " << dq.front() << std::endl;
    std::cout << "dq.back(): " << dq.back() << std::endl;
    
    // Test bounds checking with at()
    try {
        std::cout << "Attempting out of bounds access with at(10)..." << std::endl;
        dq.at(10);
    } catch (const std::out_of_range& e) {
        std::cout << "Caught exception: " << e.what() << std::endl;
    }
}

// Test iterators
void test_iterators() {
    print_line("Iterator Tests");
    
    custom::deque<int> dq;
    for (int i = 1; i <= 5; ++i) {
        dq.push_back(i * 10);
    }
    
    // Regular iterators
    std::cout << "Forward iteration: ";
    for (auto it = dq.begin(); it != dq.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    // Range-based for loop
    std::cout << "Range-based for loop: ";
    for (const auto& elem : dq) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
}

// Test push and pop operations
void test_push_pop() {
    print_line("Push/Pop Tests");
    
    custom::deque<char> dq;
    
    // Push operations
    std::cout << "Pushing elements..." << std::endl;
    dq.push_back('b');
    dq.push_back('c');
    dq.push_back('d');
    dq.push_front('a');
    
    std::cout << "Deque: ";
    for (size_t i = 0; i < dq.size(); ++i) {
        std::cout << dq[i] << " ";
    }
    std::cout << std::endl;
    
    // Pop operations
    std::cout << "After pop_front(): ";
    dq.pop_front();
    for (size_t i = 0; i < dq.size(); ++i) {
        std::cout << dq[i] << " ";
    }
    std::cout << std::endl;
    
    std::cout << "After pop_back(): ";
    dq.pop_back();
    for (size_t i = 0; i < dq.size(); ++i) {
        std::cout << dq[i] << " ";
    }
    std::cout << std::endl;
}

// Test constructors
void test_constructors() {
    print_line("Constructor Tests");
    
    // Default constructor
    custom::deque<int> dq1;
    std::cout << "dq1.size(): " << dq1.size() << std::endl;
    std::cout << "dq1.empty(): " << (dq1.empty() ? "true" : "false") << std::endl;
    
    // Count constructor
    custom::deque<int> dq2(5, 42);
    std::cout << "dq2 elements: ";
    for (size_t i = 0; i < dq2.size(); ++i) {
        std::cout << dq2[i] << " ";
    }
    std::cout << std::endl;
    
    // Copy constructor
    custom::deque<int> dq3(dq2);
    std::cout << "dq3 (copy of dq2): ";
    for (size_t i = 0; i < dq3.size(); ++i) {
        std::cout << dq3[i] << " ";
    }
    std::cout << std::endl;
    
    // Modify dq3 to verify independent copy
    dq3.push_back(100);
    std::cout << "dq3 after modification: ";
    for (size_t i = 0; i < dq3.size(); ++i) {
        std::cout << dq3[i] << " ";
    }
    std::cout << std::endl;
    
    std::cout << "dq2 unchanged: ";
    for (size_t i = 0; i < dq2.size(); ++i) {
        std::cout << dq2[i] << " ";
    }
    std::cout << std::endl;
    
    // Move constructor
    custom::deque<int> dq4(std::move(dq3));
    std::cout << "dq4 (moved from dq3): ";
    for (size_t i = 0; i < dq4.size(); ++i) {
        std::cout << dq4[i] << " ";
    }
    std::cout << std::endl;
    
    std::cout << "dq3.size() after move: " << dq3.size() << std::endl;
}

// Test resizing/growing behavior
void test_growth() {
    print_line("Growth Test");
    
    custom::deque<int> dq;
    std::cout << "Pushing elements to trigger multiple growth operations..." << std::endl;
    
    for (int i = 0; i < 20; ++i) {
        dq.push_back(i);
    }
    
    std::cout << "Final size: " << dq.size() << std::endl;
    std::cout << "First few elements: " << dq[0] << " " << dq[1] << " " << dq[2] << std::endl;
    std::cout << "Last few elements: " << dq[17] << " " << dq[18] << " " << dq[19] << std::endl;
}

// Test with different types
void test_different_types() {
    print_line("Different Types Tests");
    
    // Test with POD types
    custom::deque<int> int_dq;
    int_dq.push_back(1);
    int_dq.push_back(2);
    
    custom::deque<double> double_dq;
    double_dq.push_back(1.1);
    double_dq.push_back(2.2);
    
    // Test with class type
    custom::deque<std::string> string_dq;
    string_dq.push_back("hello");
    string_dq.push_back("world");
    
    std::cout << "int_dq: " << int_dq[0] << ", " << int_dq[1] << std::endl;
    std::cout << "double_dq: " << double_dq[0] << ", " << double_dq[1] << std::endl;
    std::cout << "string_dq: " << string_dq[0] << ", " << string_dq[1] << std::endl;
}

// Test algorithm compatibility
void test_algorithms() {
    print_line("Algorithm Tests");
    
    custom::deque<int> dq;
    for (int i = 5; i > 0; --i) {
        dq.push_back(i);
    }
    
    std::cout << "Original deque: ";
    for (const auto& elem : dq) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
    
    // Sort
    std::sort(dq.begin(), dq.end());
    std::cout << "After sort: ";
    for (const auto& elem : dq) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
    
    // Find
    auto it = std::find(dq.begin(), dq.end(), 3);
    if (it != dq.end()) {
        std::cout << "Found 3 in the deque" << std::endl;
    }
    
    // Sum
    int sum = std::accumulate(dq.begin(), dq.end(), 0);
    std::cout << "Sum of elements: " << sum << std::endl;
}

int main() {
    std::cout << "Testing custom::deque implementation" << std::endl;
    std::cout << "===================================" << std::endl;
    
    test_basic();
    test_iterators();
    test_push_pop();
    test_constructors();
    test_growth();
    test_different_types();
    test_algorithms();
    
    std::cout << "\nAll tests completed!" << std::endl;
    
    return 0;
} 