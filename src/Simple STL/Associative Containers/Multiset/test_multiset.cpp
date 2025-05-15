#include "multiset.h"
#include <iostream>
#include <cassert>
#include <string>
#include <vector>

void test_constructors() {
    std::cout << "Testing constructors...\n";
    
    // Default constructor
    custom::multiset<int> s1;
    assert(s1.empty());
    assert(s1.size() == 0);

    // Initializer list constructor
    custom::multiset<int> s2 = {1, 2, 2, 3, 3, 3};
    assert(s2.size() == 6);
    assert(s2.count(1) == 1);
    assert(s2.count(2) == 2);
    assert(s2.count(3) == 3);

    // Copy constructor
    custom::multiset<int> s3(s2);
    assert(s3.size() == 6);
    assert(s3.count(1) == 1);
    assert(s3.count(2) == 2);
    assert(s3.count(3) == 3);

    // Move constructor
    custom::multiset<int> s4(std::move(s3));
    assert(s4.size() == 6);
    assert(s3.empty()); // s3 should be empty after move
    assert(s4.count(1) == 1);
    assert(s4.count(2) == 2);
    assert(s4.count(3) == 3);

    std::cout << "Constructor tests passed!\n";
}

void test_insert_and_erase() {
    std::cout << "Testing insert and erase...\n";
    
    custom::multiset<int> s;
    
    // Test insert
    auto it1 = s.insert(1);
    assert(*it1 == 1);
    assert(s.size() == 1);

    // Test duplicate insert
    auto it2 = s.insert(1);
    assert(*it2 == 1);
    assert(s.size() == 2);
    assert(s.count(1) == 2);

    // Test multiple inserts
    s.insert(2);
    s.insert(2);
    s.insert(2);
    assert(s.size() == 5);
    assert(s.count(2) == 3);

    // Test erase
    size_t erased = s.erase(1);
    assert(erased == 2);
    assert(s.size() == 3);
    assert(!s.contains(1));

    // Test erase non-existent element
    erased = s.erase(3);
    assert(erased == 0);
    assert(s.size() == 3);

    std::cout << "Insert and erase tests passed!\n";
}

void test_iterators() {
    std::cout << "Testing iterators...\n";
    
    custom::multiset<int> s = {1, 2, 2, 3, 3, 3};

    std::vector<int> expected = {1, 2, 2, 3, 3, 3};
    std::vector<int> actual;

    // Test forward iteration
    for (const auto& value : s) {
        actual.push_back(value);
    }
    assert(actual == expected);

    // Test iterator increment
    auto it = s.begin();
    assert(*it == 1);
    ++it;
    assert(*it == 2);
    it++;
    assert(*it == 2);
    ++it;
    assert(*it == 3);

    std::cout << "Iterator tests passed!\n";
}

void test_find_and_contains() {
    std::cout << "Testing find and contains...\n";
    
    custom::multiset<int> s = {1, 2, 2, 3, 3, 3};

    // Test find existing element
    auto it = s.find(2);
    assert(it != s.end());
    assert(*it == 2);

    // Test find non-existent element
    it = s.find(4);
    assert(it == s.end());

    // Test contains
    assert(s.contains(1));
    assert(s.contains(2));
    assert(s.contains(3));
    assert(!s.contains(4));

    std::cout << "Find and contains tests passed!\n";
}

void test_custom_comparator() {
    std::cout << "Testing custom comparator...\n";
    
    custom::multiset<std::string, std::greater<std::string>> s;
    s.insert("apple");
    s.insert("banana");
    s.insert("apple");
    s.insert("cherry");

    std::vector<std::string> expected = {"cherry", "banana", "apple", "apple"};
    std::vector<std::string> actual;

    for (const auto& value : s) {
        actual.push_back(value);
    }
    assert(actual == expected);

    std::cout << "Custom comparator tests passed!\n";
}

void test_clear() {
    std::cout << "Testing clear...\n";
    
    custom::multiset<int> s = {1, 2, 2, 3, 3, 3};
    assert(s.size() == 6);
    
    s.clear();
    assert(s.empty());
    assert(s.size() == 0);
    
    // Test that we can still insert after clear
    s.insert(4);
    assert(s.size() == 1);
    assert(s.contains(4));

    std::cout << "Clear tests passed!\n";
}

int main() {
    std::cout << "Starting multiset tests...\n\n";

    test_constructors();
    test_insert_and_erase();
    test_iterators();
    test_find_and_contains();
    test_custom_comparator();
    test_clear();

    std::cout << "\nAll multiset tests passed successfully!\n";
    return 0;
} 