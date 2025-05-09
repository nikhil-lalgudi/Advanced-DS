#include "array.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <numeric>
#include <tuple>
#include <cassert>
#include <type_traits>

// Helper function for testing
void print_line(const std::string& test_name) {
    std::cout << "\n=== " << test_name << " ===" << std::endl;
}

// Test basic functionality
void test_basic() {
    print_line("Basic Tests");
    
    // Test creation and element access
    custom::array<int, 5> arr = {1, 2, 3, 4, 5};
    
    std::cout << "Array elements: ";
    for (int i = 0; i < arr.size(); ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
    
    // Test element access methods
    std::cout << "arr[2]: " << arr[2] << std::endl;
    std::cout << "arr.at(3): " << arr.at(3) << std::endl;
    std::cout << "arr.front(): " << arr.front() << std::endl;
    std::cout << "arr.back(): " << arr.back() << std::endl;
    
    // Test bounds checking with at()
    try {
        std::cout << "Attempting out of bounds access with at(10)..." << std::endl;
        arr.at(10);
    } catch (const std::out_of_range& e) {
        std::cout << "Caught exception: " << e.what() << std::endl;
    }
    
    // Test data() access
    int* raw_data = arr.data();
    std::cout << "Data pointer first element: " << *raw_data << std::endl;
}

// Test iterators
void test_iterators() {
    print_line("Iterator Tests");
    
    custom::array<int, 5> arr = {10, 20, 30, 40, 50};
    
    // Regular iterators
    std::cout << "Forward iteration: ";
    for (auto it = arr.begin(); it != arr.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    // Const iterators
    std::cout << "Const iteration: ";
    for (auto it = arr.cbegin(); it != arr.cend(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    // Reverse iterators
    std::cout << "Reverse iteration: ";
    for (auto it = arr.rbegin(); it != arr.rend(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    // Const reverse iterators
    std::cout << "Const reverse iteration: ";
    for (auto it = arr.crbegin(); it != arr.crend(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    // Range-based for loop
    std::cout << "Range-based for loop: ";
    for (const auto& elem : arr) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
}

// Test capacity methods
void test_capacity() {
    print_line("Capacity Tests");
    
    custom::array<double, 3> arr1 = {1.1, 2.2, 3.3};
    custom::array<int, 0> arr2;
    
    std::cout << "arr1.size(): " << arr1.size() << std::endl;
    std::cout << "arr1.max_size(): " << arr1.max_size() << std::endl;
    std::cout << "arr1.empty(): " << (arr1.empty() ? "true" : "false") << std::endl;
    
    std::cout << "arr2.size(): " << arr2.size() << std::endl;
    std::cout << "arr2.max_size(): " << arr2.max_size() << std::endl;
    std::cout << "arr2.empty(): " << (arr2.empty() ? "true" : "false") << std::endl;
}

// Test operations
void test_operations() {
    print_line("Operations Tests");
    
    custom::array<char, 5> arr1 = {'a', 'b', 'c', 'd', 'e'};
    custom::array<char, 5> arr2 = {'v', 'w', 'x', 'y', 'z'};
    
    std::cout << "Before fill, arr1: ";
    for (const auto& c : arr1) {
        std::cout << c << " ";
    }
    std::cout << std::endl;
    
    arr1.fill('X');
    
    std::cout << "After fill, arr1: ";
    for (const auto& c : arr1) {
        std::cout << c << " ";
    }
    std::cout << std::endl;
    
    std::cout << "Before swap, arr2: ";
    for (const auto& c : arr2) {
        std::cout << c << " ";
    }
    std::cout << std::endl;
    
    arr1.swap(arr2);
    
    std::cout << "After swap, arr1: ";
    for (const auto& c : arr1) {
        std::cout << c << " ";
    }
    std::cout << std::endl;
    
    std::cout << "After swap, arr2: ";
    for (const auto& c : arr2) {
        std::cout << c << " ";
    }
    std::cout << std::endl;
    
    // Test non-member swap
    swap(arr1, arr2);
    
    std::cout << "After non-member swap, arr1: ";
    for (const auto& c : arr1) {
        std::cout << c << " ";
    }
    std::cout << std::endl;
}

// Test comparison operators
void test_comparisons() {
    print_line("Comparison Tests");
    
    custom::array<int, 3> a1 = {1, 2, 3};
    custom::array<int, 3> a2 = {1, 2, 3};
    custom::array<int, 3> a3 = {1, 2, 4};
    custom::array<int, 3> a4 = {1, 1, 5};
    
    std::cout << "a1 == a2: " << (a1 == a2 ? "true" : "false") << std::endl;
    std::cout << "a1 != a3: " << (a1 != a3 ? "true" : "false") << std::endl;
    std::cout << "a1 < a3: " << (a1 < a3 ? "true" : "false") << std::endl;
    std::cout << "a3 > a1: " << (a3 > a1 ? "true" : "false") << std::endl;
    std::cout << "a4 < a1: " << (a4 < a1 ? "true" : "false") << std::endl;
    std::cout << "a1 <= a2: " << (a1 <= a2 ? "true" : "false") << std::endl;
    std::cout << "a3 >= a1: " << (a3 >= a1 ? "true" : "false") << std::endl;
}

// Test integration with algorithms
void test_algorithms() {
    print_line("Algorithm Tests");
    
    custom::array<int, 5> arr = {5, 2, 1, 4, 3};
    
    std::cout << "Original array: ";
    for (const auto& elem : arr) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
    
    // Sort
    std::sort(arr.begin(), arr.end());
    std::cout << "After sort: ";
    for (const auto& elem : arr) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
    
    // Find
    auto it = std::find(arr.begin(), arr.end(), 4);
    if (it != arr.end()) {
        std::cout << "Found 4 at position: " << std::distance(arr.begin(), it) << std::endl;
    }
    
    // Accumulate
    int sum = std::accumulate(arr.begin(), arr.end(), 0);
    std::cout << "Sum of elements: " << sum << std::endl;
    
    // Transform
    custom::array<int, 5> result;
    std::transform(arr.begin(), arr.end(), result.begin(), 
                   [](int x) { return x * 2; });
    
    std::cout << "After transform (x2): ";
    for (const auto& elem : result) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
}

// Test tuple interface
void test_tuple_interface() {
    print_line("Tuple Interface Tests");
    
    custom::array<int, 3> arr = {100, 200, 300};
    
    // Test std::get
    std::cout << "std::get<0>(arr): " << std::get<0>(arr) << std::endl;
    std::cout << "std::get<1>(arr): " << std::get<1>(arr) << std::endl;
    std::cout << "std::get<2>(arr): " << std::get<2>(arr) << std::endl;
    
    // Test custom::get
    std::cout << "custom::get<0>(arr): " << custom::get<0>(arr) << std::endl;
    std::cout << "custom::get<1>(arr): " << custom::get<1>(arr) << std::endl;
    std::cout << "custom::get<2>(arr): " << custom::get<2>(arr) << std::endl;
    
    // Test tuple_size
    std::cout << "std::tuple_size<decltype(arr)>::value: " 
              << std::tuple_size<decltype(arr)>::value << std::endl;
    
    // Test structured binding (C++17)
    auto& [x, y, z] = arr;
    std::cout << "Structured binding: " << x << ", " << y << ", " << z << std::endl;
    
    // Modify through the reference
    x = 1000;
    std::cout << "After modification: " << arr[0] << std::endl;
}

// Test zero-sized arrays
void test_zero_sized() {
    print_line("Zero-sized Array Tests");
    
    custom::array<int, 0> empty;
    
    std::cout << "empty.size(): " << empty.size() << std::endl;
    std::cout << "empty.empty(): " << (empty.empty() ? "true" : "false") << std::endl;
    
    // Test iterators on empty array
    std::cout << "empty.begin() == empty.end(): " 
              << (empty.begin() == empty.end() ? "true" : "false") << std::endl;
    
    // These operations should not crash but have UB
    std::cout << "Testing various methods on empty array..." << std::endl;
    auto dummy1 = empty.front();  // UB
    auto dummy2 = empty.back();   // UB
    auto dummy3 = empty[0];       // UB
    (void)dummy1; (void)dummy2; (void)dummy3;  // Avoid unused variable warnings
    
    // This should throw
    try {
        empty.at(0);
        std::cout << "ERROR: empty.at(0) did not throw!" << std::endl;
    } catch (const std::out_of_range& e) {
        std::cout << "Correctly caught exception: " << e.what() << std::endl;
    }
}

// Test C++17 deduction guides
void test_deduction_guides() {
    print_line("Deduction Guides Tests");
    
    // Use deduction guide to create array
    custom::array arr1{1, 2, 3, 4, 5};  // deduces custom::array<int, 5>
    
    std::cout << "arr1 size: " << arr1.size() << std::endl;
    std::cout << "arr1 element type: " << typeid(arr1[0]).name() << std::endl;
    
    custom::array arr2{'a', 'b', 'c'};  // deduces custom::array<char, 3>
    
    std::cout << "arr2 size: " << arr2.size() << std::endl;
    std::cout << "arr2 element type: " << typeid(arr2[0]).name() << std::endl;
    
    // Mixed types follow the same rules as std::array
    custom::array arr3{1, 2, 3.0};  // deduces to custom::array<double, 3>
    
    std::cout << "arr3 size: " << arr3.size() << std::endl;
    std::cout << "arr3 element type: " << typeid(arr3[0]).name() << std::endl;
}

// Test compiling with different types
void test_different_types() {
    print_line("Different Types Tests");
    
    // Test with POD types
    custom::array<int, 3> int_arr = {1, 2, 3};
    custom::array<double, 2> double_arr = {1.1, 2.2};
    custom::array<char, 4> char_arr = {'a', 'b', 'c', 'd'};
    
    // Test with class type
    custom::array<std::string, 2> string_arr = {"hello", "world"};
    
    std::cout << "int_arr: " << int_arr[0] << ", " << int_arr[1] << ", " << int_arr[2] << std::endl;
    std::cout << "double_arr: " << double_arr[0] << ", " << double_arr[1] << std::endl;
    std::cout << "char_arr: " << char_arr[0] << ", " << char_arr[1] << ", " 
              << char_arr[2] << ", " << char_arr[3] << std::endl;
    std::cout << "string_arr: " << string_arr[0] << ", " << string_arr[1] << std::endl;
}

// Compile-time verifications
void test_constexpr() {
    print_line("Constexpr Tests");
    
    // Verify constexpr creation
    constexpr custom::array<int, 3> arr = {1, 2, 3};
    
    // Verify constexpr element access
    static_assert(arr[0] == 1, "arr[0] should be 1");
    static_assert(arr[1] == 2, "arr[1] should be 2");
    static_assert(arr[2] == 3, "arr[2] should be 3");
    
    // Other constexpr tests
    static_assert(arr.size() == 3, "arr.size() should be 3");
    static_assert(!arr.empty(), "arr should not be empty");
    static_assert(arr.front() == 1, "arr.front() should be 1");
    static_assert(arr.back() == 3, "arr.back() should be 3");
    
    std::cout << "All static_assert tests passed!" << std::endl;
}

// Helper function to print array
template<typename T, size_t N>
void print_array(const custom::array<T, N>& arr, const std::string& name) {
    std::cout << name << ": ";
    for (const auto& elem : arr) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
}

// Test comparison with std::array
void test_std_array_comparison() {
    print_line("std::array Comparison Tests");
    
    custom::array<int, 5> custom_arr = {1, 2, 3, 4, 5};
    std::array<int, 5> std_arr = {1, 2, 3, 4, 5};
    
    // Compare basic operations
    std::cout << "Size comparison: " 
              << "custom_arr.size() = " << custom_arr.size() << ", "
              << "std_arr.size() = " << std_arr.size() << std::endl;
    
    std::cout << "Element access comparison: "
              << "custom_arr[2] = " << custom_arr[2] << ", "
              << "std_arr[2] = " << std_arr[2] << std::endl;
    
    std::cout << "Front/back comparison: "
              << "custom_arr.front() = " << custom_arr.front() << ", "
              << "std_arr.front() = " << std_arr.front() << ", "
              << "custom_arr.back() = " << custom_arr.back() << ", "
              << "std_arr.back() = " << std_arr.back() << std::endl;
    
    // Sort both and compare
    std::sort(custom_arr.begin(), custom_arr.end(), std::greater<int>());
    std::sort(std_arr.begin(), std_arr.end(), std::greater<int>());
    
    std::cout << "After sorting in descending order:" << std::endl;
    print_array(custom_arr, "custom_arr");
    std::cout << "std_arr: ";
    for (const auto& elem : std_arr) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
}

int main() {
    std::cout << "Testing custom::array implementation" << std::endl;
    std::cout << "====================================" << std::endl;
    
    test_basic();
    test_iterators();
    test_capacity();
    test_operations();
    test_comparisons();
    test_algorithms();
    test_tuple_interface();
    test_zero_sized();
    test_deduction_guides();
    test_different_types();
    test_constexpr();
    test_std_array_comparison();
    
    std::cout << "\nAll tests completed successfully!" << std::endl;
    
    return 0;
}