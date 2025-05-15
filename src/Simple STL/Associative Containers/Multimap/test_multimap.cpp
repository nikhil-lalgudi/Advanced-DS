#include "multimap.h"
#include <iostream>
#include <cassert>
#include <string>
#include <vector>

void test_constructors() {
    std::cout << "Testing constructors...\n";
    
    // Default constructor
    custom::multimap<std::string, int> m1;
    assert(m1.empty());
    assert(m1.size() == 0);

    // Initializer list constructor
    custom::multimap<std::string, int> m2 = {
        {"apple", 1},
        {"banana", 2},
        {"apple", 3},
        {"cherry", 4},
        {"apple", 5}
    };
    assert(m2.size() == 5);
    assert(m2.count("apple") == 3);
    assert(m2.count("banana") == 1);
    assert(m2.count("cherry") == 1);

    // Copy constructor
    custom::multimap<std::string, int> m3(m2);
    assert(m3.size() == 5);
    assert(m3.count("apple") == 3);
    assert(m3.count("banana") == 1);
    assert(m3.count("cherry") == 1);

    // Move constructor
    custom::multimap<std::string, int> m4(std::move(m3));
    assert(m4.size() == 5);
    assert(m3.empty()); // m3 should be empty after move
    assert(m4.count("apple") == 3);
    assert(m4.count("banana") == 1);
    assert(m4.count("cherry") == 1);

    std::cout << "Constructor tests passed!\n";
}

void test_insert_and_erase() {
    std::cout << "Testing insert and erase...\n";
    
    custom::multimap<std::string, int> m;
    
    // Test insert
    auto it1 = m.insert({"apple", 1});
    assert(it1->first == "apple");
    assert(it1->second == 1);
    assert(m.size() == 1);

    // Test duplicate insert
    auto it2 = m.insert({"apple", 2});
    assert(it2->first == "apple");
    assert(it2->second == 2);
    assert(m.size() == 2);
    assert(m.count("apple") == 2);

    // Test multiple inserts
    m.insert({"banana", 3});
    m.insert({"banana", 4});
    m.insert({"banana", 5});
    assert(m.size() == 5);
    assert(m.count("banana") == 3);

    // Test erase
    size_t erased = m.erase("apple");
    assert(erased == 2);
    assert(m.size() == 3);
    assert(!m.contains("apple"));

    // Test erase non-existent element
    erased = m.erase("cherry");
    assert(erased == 0);
    assert(m.size() == 3);

    std::cout << "Insert and erase tests passed!\n";
}

void test_iterators() {
    std::cout << "Testing iterators...\n";
    
    custom::multimap<std::string, int> m = {
        {"apple", 1},
        {"apple", 2},
        {"banana", 3},
        {"banana", 4},
        {"cherry", 5}
    };

    std::vector<std::pair<std::string, int>> expected = {
        {"apple", 1},
        {"apple", 2},
        {"banana", 3},
        {"banana", 4},
        {"cherry", 5}
    };
    std::vector<std::pair<std::string, int>> actual;

    // Test forward iteration
    for (const auto& [key, value] : m) {
        actual.push_back({key, value});
    }
    assert(actual == expected);

    // Test iterator increment
    auto it = m.begin();
    assert(it->first == "apple");
    assert(it->second == 1);
    ++it;
    assert(it->first == "apple");
    assert(it->second == 2);
    it++;
    assert(it->first == "banana");
    assert(it->second == 3);

    std::cout << "Iterator tests passed!\n";
}

void test_find_and_contains() {
    std::cout << "Testing find and contains...\n";
    
    custom::multimap<std::string, int> m = {
        {"apple", 1},
        {"apple", 2},
        {"banana", 3},
        {"banana", 4},
        {"cherry", 5}
    };

    // Test find existing element
    auto it = m.find("banana");
    assert(it != m.end());
    assert(it->first == "banana");
    assert(it->second == 3);

    // Test find non-existent element
    it = m.find("fig");
    assert(it == m.end());

    // Test contains
    assert(m.contains("apple"));
    assert(m.contains("banana"));
    assert(m.contains("cherry"));
    assert(!m.contains("fig"));

    std::cout << "Find and contains tests passed!\n";
}

void test_custom_comparator() {
    std::cout << "Testing custom comparator...\n";
    
    custom::multimap<std::string, int, std::greater<std::string>> m;
    m.insert({"apple", 1});
    m.insert({"banana", 2});
    m.insert({"apple", 3});
    m.insert({"cherry", 4});

    std::vector<std::pair<std::string, int>> expected = {
        {"cherry", 4},
        {"banana", 2},
        {"apple", 1},
        {"apple", 3}
    };
    std::vector<std::pair<std::string, int>> actual;

    for (const auto& [key, value] : m) {
        actual.push_back({key, value});
    }
    assert(actual == expected);

    std::cout << "Custom comparator tests passed!\n";
}

void test_clear() {
    std::cout << "Testing clear...\n";
    
    custom::multimap<std::string, int> m = {
        {"apple", 1},
        {"apple", 2},
        {"banana", 3},
        {"banana", 4},
        {"cherry", 5}
    };
    assert(m.size() == 5);
    
    m.clear();
    assert(m.empty());
    assert(m.size() == 0);
    
    // Test that we can still insert after clear
    m.insert({"date", 6});
    assert(m.size() == 1);
    assert(m.contains("date"));

    std::cout << "Clear tests passed!\n";
}

int main() {
    std::cout << "Starting multimap tests...\n\n";

    test_constructors();
    test_insert_and_erase();
    test_iterators();
    test_find_and_contains();
    test_custom_comparator();
    test_clear();

    std::cout << "\nAll multimap tests passed successfully!\n";
    return 0;
} 