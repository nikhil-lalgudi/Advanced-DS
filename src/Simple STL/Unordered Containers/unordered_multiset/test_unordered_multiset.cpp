#include "unordered_multiset.h"
#include <cassert>
#include <iostream>
#include <string>

void test_constructors() {
    std::cout << "Testing constructors... ";

    // Default constructor
    custom::unordered_multiset<std::string> set1;
    assert(set1.empty());
    assert(set1.size() == 0);

    // Initializer list constructor
    custom::unordered_multiset<std::string> set2{"apple", "banana", "cherry", "banana"};
    assert(set2.size() == 4);
    assert(set2.count("apple") == 1);
    assert(set2.count("banana") == 2);
    assert(set2.count("cherry") == 1);

    // Copy constructor
    custom::unordered_multiset<std::string> set3(set2);
    assert(set3.size() == 4);
    assert(set3.count("apple") == 1);
    assert(set3.count("banana") == 2);
    assert(set3.count("cherry") == 1);

    // Move constructor
    custom::unordered_multiset<std::string> set4(std::move(set3));
    assert(set4.size() == 4);
    assert(set4.count("apple") == 1);
    assert(set4.count("banana") == 2);
    assert(set4.count("cherry") == 1);
    assert(set3.empty());

    std::cout << "Passed!" << std::endl;
}

void test_insert_and_erase() {
    std::cout << "Testing insert and erase... ";

    custom::unordered_multiset<std::string> set;

    // Insert elements
    auto it1 = set.insert("apple");
    assert(*it1 == "apple");

    auto it2 = set.insert("banana");
    assert(*it2 == "banana");

    // Insert duplicate element
    auto it3 = set.insert("apple");
    assert(*it3 == "apple");
    assert(set.count("apple") == 2);

    // Erase one occurrence
    size_t erased = set.erase("apple");
    assert(erased == 1);
    assert(set.size() == 2);
    assert(set.count("apple") == 1);

    // Erase all occurrences
    erased = set.erase("apple");
    assert(erased == 1);
    assert(set.size() == 1);
    assert(set.count("apple") == 0);

    // Try to erase non-existent element
    erased = set.erase("fig");
    assert(erased == 0);
    assert(set.size() == 1);

    std::cout << "Passed!" << std::endl;
}

void test_iterators() {
    std::cout << "Testing iterators... ";

    custom::unordered_multiset<std::string> set{"apple", "banana", "cherry", "banana"};

    // Test forward iteration
    int count = 0;
    for (const auto& element : set) {
        ++count;
    }
    assert(count == 4);

    // Test iterator increment
    auto it = set.begin();
    assert(it != set.end());
    ++it;
    assert(it != set.end());
    ++it;
    assert(it != set.end());
    ++it;
    assert(it != set.end());
    ++it;
    assert(it == set.end());

    std::cout << "Passed!" << std::endl;
}

void test_find_and_contains() {
    std::cout << "Testing find and contains... ";

    custom::unordered_multiset<std::string> set{"apple", "banana", "cherry", "banana"};

    // Test find
    auto it1 = set.find("banana");
    assert(it1 != set.end());
    assert(*it1 == "banana");

    auto it2 = set.find("fig");
    assert(it2 == set.end());

    // Test contains
    assert(set.contains("apple"));
    assert(set.contains("banana"));
    assert(set.contains("cherry"));
    assert(!set.contains("fig"));

    std::cout << "Passed!" << std::endl;
}

void test_custom_hash() {
    std::cout << "Testing custom hash function... ";

    struct StringHash {
        size_t operator()(const std::string& s) const {
            size_t hash = 0;
            for (char c : s) {
                hash = hash * 31 + c;
            }
            return hash;
        }
    };

    custom::unordered_multiset<std::string, StringHash> set;
    set.insert("apple");
    set.insert("banana");
    set.insert("cherry");
    set.insert("banana");

    assert(set.size() == 4);
    assert(set.count("apple") == 1);
    assert(set.count("banana") == 2);
    assert(set.count("cherry") == 1);

    std::cout << "Passed!" << std::endl;
}

void test_clear() {
    std::cout << "Testing clear... ";

    custom::unordered_multiset<std::string> set{"apple", "banana", "cherry", "banana"};

    assert(set.size() == 4);
    set.clear();
    assert(set.empty());
    assert(set.size() == 0);

    // Test that we can still insert after clearing
    set.insert("date");
    assert(set.size() == 1);
    assert(set.contains("date"));

    std::cout << "Passed!" << std::endl;
}

void test_load_factor() {
    std::cout << "Testing load factor... ";

    custom::unordered_multiset<int> set;
    set.max_load_factor(0.5f);

    // Insert elements to trigger rehash
    for (int i = 0; i < 100; ++i) {
        set.insert(i);
        set.insert(i); // Insert duplicates
    }

    assert(set.size() == 200);
    assert(set.load_factor() <= set.max_load_factor());

    std::cout << "Passed!" << std::endl;
}

int main() {
    std::cout << "Starting unordered_multiset tests...\n" << std::endl;

    test_constructors();
    test_insert_and_erase();
    test_iterators();
    test_find_and_contains();
    test_custom_hash();
    test_clear();
    test_load_factor();

    std::cout << "\nAll unordered_multiset tests passed successfully!" << std::endl;
    return 0;
} 