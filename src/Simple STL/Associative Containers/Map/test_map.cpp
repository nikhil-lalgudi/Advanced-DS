#include "map.h"
#include <iostream>
#include <cassert>
#include <string>
#include <vector>

void test_constructors() {
    std::cout << "Testing constructors...\n";
    
    // Default constructor
    custom::map<std::string, int> m1;
    assert(m1.empty());
    assert(m1.size() == 0);

    // Initializer list constructor
    custom::map<std::string, int> m2 = {
        {"apple", 1},
        {"banana", 2},
        {"cherry", 3}
    };
    assert(m2.size() == 3);
    assert(m2.contains("apple"));
    assert(m2.contains("cherry"));

    // Copy constructor
    custom::map<std::string, int> m3(m2);
    assert(m3.size() == 3);
    assert(m3.contains("apple"));
    assert(m3.contains("cherry"));

    // Move constructor
    custom::map<std::string, int> m4(std::move(m3));
    assert(m4.size() == 3);
    assert(m3.empty()); // m3 should be empty after move
    assert(m4.contains("apple"));
    assert(m4.contains("cherry"));

    std::cout << "Constructor tests passed!\n";
}

void test_insert_and_erase() {
    std::cout << "Testing insert and erase...\n";
    
    custom::map<std::string, int> m;
    
    // Test insert
    auto [it1, success1] = m.insert({"apple", 1});
    assert(success1);
    assert(it1->first == "apple");
    assert(it1->second == 1);
    assert(m.size() == 1);

    // Test duplicate insert
    auto [it2, success2] = m.insert({"apple", 2});
    assert(!success2);
    assert(m.size() == 1);
    assert(m["apple"] == 1);

    // Test multiple inserts
    m.insert({"banana", 2});
    m.insert({"cherry", 3});
    assert(m.size() == 3);

    // Test erase
    size_t erased = m.erase("apple");
    assert(erased == 1);
    assert(m.size() == 2);
    assert(!m.contains("apple"));

    // Test erase non-existent element
    erased = m.erase("fig");
    assert(erased == 0);
    assert(m.size() == 2);

    std::cout << "Insert and erase tests passed!\n";
}

void test_element_access() {
    std::cout << "Testing element access...\n";
    
    custom::map<std::string, int> m;
    
    // Test operator[]
    m["apple"] = 1;
    assert(m["apple"] == 1);
    assert(m.size() == 1);

    // Test at()
    m.insert({"banana", 2});
    assert(m.at("banana") == 2);
    
    // Test at() with non-existent key
    try {
        m.at("cherry");
        assert(false); // Should not reach here
    } catch (const std::out_of_range&) {
        // Expected exception
    }

    std::cout << "Element access tests passed!\n";
}

void test_iterators() {
    std::cout << "Testing iterators...\n";
    
    custom::map<std::string, int> m = {
        {"apple", 1},
        {"banana", 2},
        {"cherry", 3}
    };

    std::vector<std::pair<std::string, int>> expected = {
        {"apple", 1},
        {"banana", 2},
        {"cherry", 3}
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
    assert(it->first == "banana");
    assert(it->second == 2);
    it++;
    assert(it->first == "cherry");
    assert(it->second == 3);

    std::cout << "Iterator tests passed!\n";
}

void test_find_and_contains() {
    std::cout << "Testing find and contains...\n";
    
    custom::map<std::string, int> m = {
        {"apple", 1},
        {"banana", 2},
        {"cherry", 3}
    };

    // Test find existing element
    auto it = m.find("banana");
    assert(it != m.end());
    assert(it->first == "banana");
    assert(it->second == 2);

    // Test find non-existent element
    it = m.find("fig");
    assert(it == m.end());

    // Test contains
    assert(m.contains("apple"));
    assert(m.contains("cherry"));
    assert(!m.contains("fig"));

    std::cout << "Find and contains tests passed!\n";
}

void test_custom_comparator() {
    std::cout << "Testing custom comparator...\n";
    
    custom::map<std::string, int, std::greater<std::string>> m;
    m.insert({"apple", 1});
    m.insert({"banana", 2});
    m.insert({"cherry", 3});

    std::vector<std::pair<std::string, int>> expected = {
        {"cherry", 3},
        {"banana", 2},
        {"apple", 1}
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
    
    custom::map<std::string, int> m = {
        {"apple", 1},
        {"banana", 2},
        {"cherry", 3}
    };
    assert(m.size() == 3);
    
    m.clear();
    assert(m.empty());
    assert(m.size() == 0);
    
    // Test that we can still insert after clear
    m.insert({"date", 4});
    assert(m.size() == 1);
    assert(m.contains("date"));

    std::cout << "Clear tests passed!\n";
}

int main() {
    std::cout << "Starting map tests...\n\n";

    test_constructors();
    test_insert_and_erase();
    test_element_access();
    test_iterators();
    test_find_and_contains();
    test_custom_comparator();
    test_clear();

    std::cout << "\nAll map tests passed successfully!\n";
    return 0;
} 