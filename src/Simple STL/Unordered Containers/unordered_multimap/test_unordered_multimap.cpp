#include "unordered_multimap.h"
#include <cassert>
#include <iostream>
#include <string>

void test_constructors() {
    std::cout << "Testing constructors... ";

    // Default constructor
    custom::unordered_multimap<std::string, int> map1;
    assert(map1.empty());
    assert(map1.size() == 0);

    // Initializer list constructor
    custom::unordered_multimap<std::string, int> map2{
        {"apple", 1},
        {"banana", 2},
        {"apple", 3}
    };
    assert(map2.size() == 3);
    assert(map2.count("apple") == 2);
    assert(map2.count("banana") == 1);

    // Copy constructor
    custom::unordered_multimap<std::string, int> map3(map2);
    assert(map3.size() == 3);
    assert(map3.count("apple") == 2);
    assert(map3.count("banana") == 1);

    // Move constructor
    custom::unordered_multimap<std::string, int> map4(std::move(map3));
    assert(map4.size() == 3);
    assert(map4.count("apple") == 2);
    assert(map4.count("banana") == 1);
    assert(map3.empty());

    std::cout << "Passed!" << std::endl;
}

void test_insert_and_erase() {
    std::cout << "Testing insert and erase... ";

    custom::unordered_multimap<std::string, int> map;

    // Insert elements
    map.insert({"apple", 1});
    map.insert({"banana", 2});
    map.insert({"apple", 3});
    assert(map.size() == 3);
    assert(map.count("apple") == 2);
    assert(map.count("banana") == 1);

    // Erase elements
    size_t erased = map.erase("apple");
    assert(erased == 2);
    assert(map.size() == 1);
    assert(map.count("apple") == 0);
    assert(map.count("banana") == 1);

    // Erase non-existent element
    erased = map.erase("orange");
    assert(erased == 0);
    assert(map.size() == 1);

    std::cout << "Passed!" << std::endl;
}

void test_iterators() {
    std::cout << "Testing iterators... ";

    custom::unordered_multimap<std::string, int> map{
        {"apple", 1},
        {"banana", 2},
        {"apple", 3}
    };

    // Test forward iteration
    int sum = 0;
    for (const auto& pair : map) {
        sum += pair.second;
    }
    assert(sum == 6);

    // Test iterator increment
    auto it = map.begin();
    assert(it != map.end());
    ++it;
    assert(it != map.end());
    ++it;
    assert(it != map.end());
    ++it;
    assert(it == map.end());

    std::cout << "Passed!" << std::endl;
}

void test_find_and_contains() {
    std::cout << "Testing find and contains... ";

    custom::unordered_multimap<std::string, int> map{
        {"apple", 1},
        {"banana", 2},
        {"apple", 3}
    };

    // Test find
    auto it = map.find("apple");
    assert(it != map.end());
    assert(it->first == "apple");
    assert(it->second == 1 || it->second == 3);

    it = map.find("orange");
    assert(it == map.end());

    // Test contains
    assert(map.contains("apple"));
    assert(map.contains("banana"));
    assert(!map.contains("orange"));

    std::cout << "Passed!" << std::endl;
}

void test_custom_hash() {
    std::cout << "Testing custom hash... ";

    struct StringHash {
        size_t operator()(const std::string& s) const {
            size_t hash = 0;
            for (char c : s) {
                hash = hash * 31 + c;
            }
            return hash;
        }
    };

    custom::unordered_multimap<std::string, int, StringHash> map;
    map.insert({"apple", 1});
    map.insert({"banana", 2});
    map.insert({"apple", 3});

    assert(map.size() == 3);
    assert(map.count("apple") == 2);
    assert(map.count("banana") == 1);

    std::cout << "Passed!" << std::endl;
}

void test_clear() {
    std::cout << "Testing clear... ";

    custom::unordered_multimap<std::string, int> map{
        {"apple", 1},
        {"banana", 2},
        {"apple", 3}
    };

    assert(map.size() == 3);
    map.clear();
    assert(map.empty());
    assert(map.size() == 0);

    // Test that we can still insert after clearing
    map.insert({"orange", 4});
    assert(map.size() == 1);
    assert(map.count("orange") == 1);

    std::cout << "Passed!" << std::endl;
}

void test_load_factor() {
    std::cout << "Testing load factor... ";

    custom::unordered_multimap<std::string, int> map;
    assert(map.load_factor() == 0.0f);
    assert(map.max_load_factor() == 1.0f);

    // Insert elements to increase load factor
    for (int i = 0; i < 20; ++i) {
        map.insert({"key" + std::to_string(i), i});
    }

    assert(map.load_factor() > 0.0f);
    assert(map.load_factor() <= map.max_load_factor());

    // Change max load factor
    map.max_load_factor(0.5f);
    assert(map.max_load_factor() == 0.5f);

    std::cout << "Passed!" << std::endl;
}

int main() {
    std::cout << "Starting unordered_multimap tests...\n" << std::endl;

    test_constructors();
    test_insert_and_erase();
    test_iterators();
    test_find_and_contains();
    test_custom_hash();
    test_clear();
    test_load_factor();

    std::cout << "\nAll unordered_multimap tests passed successfully!" << std::endl;
    return 0;
} 