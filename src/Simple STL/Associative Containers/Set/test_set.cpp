#include "set.h"
#include <iostream>
#include <cassert>
#include <string>
#include <vector>

void test_constructors() {
    std::cout << "Testing constructors...\n";
    
    // Default constructor
    custom::set<int> s1;
    assert(s1.empty());
    assert(s1.size() == 0);

    // Initializer list constructor
    custom::set<int> s2 = {1, 2, 3, 4, 5};
    assert(s2.size() == 5);
    assert(s2.contains(1));
    assert(s2.contains(5));

    // Copy constructor
    custom::set<int> s3(s2);
    assert(s3.size() == 5);
    assert(s3.contains(1));
    assert(s3.contains(5));

    // Move constructor
    custom::set<int> s4(std::move(s3));
    assert(s4.size() == 5);
    assert(s3.empty()); // s3 should be empty after move
    assert(s4.contains(1));
    assert(s4.contains(5));

    std::cout << "Constructor tests passed!\n";
}

void test_insert_and_erase() {
    std::cout << "Testing insert and erase...\n";
    
    custom::set<int> s;
    
    // Test insert
    auto [it1, success1] = s.insert(10);
    assert(success1);
    assert(*it1 == 10);
    assert(s.size() == 1);

    // Test duplicate insert
    auto [it2, success2] = s.insert(10);
    assert(!success2);
    assert(s.size() == 1);

    // Test multiple inserts
    s.insert(20);
    s.insert(5);
    s.insert(15);
    assert(s.size() == 4);

    // Test erase
    size_t erased = s.erase(10);
    assert(erased == 1);
    assert(s.size() == 3);
    assert(!s.contains(10));

    // Test erase non-existent element
    erased = s.erase(100);
    assert(erased == 0);
    assert(s.size() == 3);

    std::cout << "Insert and erase tests passed!\n";
}

void test_iterators() {
    std::cout << "Testing iterators...\n";
    
    custom::set<int> s = {1, 2, 3, 4, 5};
    std::vector<int> expected = {1, 2, 3, 4, 5};
    std::vector<int> actual;

    // Test forward iteration
    for (int val : s) {
        actual.push_back(val);
    }
    assert(actual == expected);

    // Test iterator increment
    auto it = s.begin();
    assert(*it == 1);
    ++it;
    assert(*it == 2);
    it++;
    assert(*it == 3);

    std::cout << "Iterator tests passed!\n";
}

void test_find_and_contains() {
    std::cout << "Testing find and contains...\n";
    
    custom::set<int> s = {1, 2, 3, 4, 5};

    // Test find existing element
    auto it = s.find(3);
    assert(it != s.end());
    assert(*it == 3);

    // Test find non-existent element
    it = s.find(10);
    assert(it == s.end());

    // Test contains
    assert(s.contains(1));
    assert(s.contains(5));
    assert(!s.contains(10));

    std::cout << "Find and contains tests passed!\n";
}

void test_custom_comparator() {
    std::cout << "Testing custom comparator...\n";
    
    custom::set<std::string, std::greater<std::string>> s;
    s.insert("apple");
    s.insert("banana");
    s.insert("cherry");

    std::vector<std::string> expected = {"cherry", "banana", "apple"};
    std::vector<std::string> actual;

    for (const auto& str : s) {
        actual.push_back(str);
    }
    assert(actual == expected);

    std::cout << "Custom comparator tests passed!\n";
}

void test_clear() {
    std::cout << "Testing clear...\n";
    
    custom::set<int> s = {1, 2, 3, 4, 5};
    assert(s.size() == 5);
    
    s.clear();
    assert(s.empty());
    assert(s.size() == 0);
    
    // Test that we can still insert after clear
    s.insert(10);
    assert(s.size() == 1);
    assert(s.contains(10));

    std::cout << "Clear tests passed!\n";
}

int main() {
    std::cout << "Starting set tests...\n\n";

    test_constructors();
    test_insert_and_erase();
    test_iterators();
    test_find_and_contains();
    test_custom_comparator();
    test_clear();

    std::cout << "\nAll set tests passed successfully!\n";
    return 0;
} 