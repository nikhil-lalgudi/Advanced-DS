#include "unordered_map.h"
#include <cassert>
#include <iostream>
#include <string>

void test_constructors() {
    std::cout << "Testing constructors... ";

    // Default constructor
    custom::unordered_map<std::string, int> map1;
    assert(map1.empty());
    assert(map1.size() == 0);

    // Initializer list constructor
    custom::unordered_map<std::string, int> map2{
        {"apple", 1},
        {"banana", 2},
        {"cherry", 3}
    };
    assert(map2.size() == 3);
    assert(map2["apple"] == 1);
    assert(map2["banana"] == 2);
    assert(map2["cherry"] == 3);

    // Copy constructor
    custom::unordered_map<std::string, int> map3(map2);
    assert(map3.size() == 3);
    assert(map3["apple"] == 1);
    assert(map3["banana"] == 2);
    assert(map3["cherry"] == 3);

    // Move constructor
    custom::unordered_map<std::string, int> map4(std::move(map3));
    assert(map4.size() == 3);
    assert(map4["apple"] == 1);
    assert(map4["banana"] == 2);
    assert(map4["cherry"] == 3);
    assert(map3.empty());

    std::cout << "Passed!" << std::endl;
}

void test_insert_and_erase() {
    std::cout << "Testing insert and erase... ";

    custom::unordered_map<std::string, int> map;

    // Insert elements
    auto [it1, success1] = map.insert({"apple", 1});
    assert(success1);
    assert(it1->first == "apple");
    assert(it1->second == 1);

    auto [it2, success2] = map.insert({"banana", 2});
    assert(success2);
    assert(it2->first == "banana");
    assert(it2->second == 2);

    // Try to insert duplicate key
    auto [it3, success3] = map.insert({"apple", 3});
    assert(!success3);
    assert(it3->first == "apple");
    assert(it3->second == 1);

    // Erase existing element
    size_t erased = map.erase("apple");
    assert(erased == 1);
    assert(map.size() == 1);
    assert(!map.contains("apple"));

    // Try to erase non-existent element
    erased = map.erase("fig");
    assert(erased == 0);
    assert(map.size() == 1);

    std::cout << "Passed!" << std::endl;
}

void test_element_access() {
    std::cout << "Testing element access... ";

    custom::unordered_map<std::string, int> map;

    // Test operator[]
    map["apple"] = 1;
    assert(map["apple"] == 1);
    assert(map["banana"] == 0); // Creates new element with default value

    // Test at()
    assert(map.at("apple") == 1);
    try {
        map.at("cherry");
        assert(false); // Should not reach here
    } catch (const std::out_of_range&) {
        // Expected exception
    }

    std::cout << "Passed!" << std::endl;
}

void test_iterators() {
    std::cout << "Testing iterators... ";

    custom::unordered_map<std::string, int> map{
        {"apple", 1},
        {"banana", 2},
        {"cherry", 3}
    };

    // Test forward iteration
    int sum = 0;
    for (const auto& [key, value] : map) {
        sum += value;
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

    custom::unordered_map<std::string, int> map{
        {"apple", 1},
        {"banana", 2},
        {"cherry", 3}
    };

    // Test find
    auto it1 = map.find("banana");
    assert(it1 != map.end());
    assert(it1->first == "banana");
    assert(it1->second == 2);

    auto it2 = map.find("fig");
    assert(it2 == map.end());

    // Test contains
    assert(map.contains("apple"));
    assert(map.contains("banana"));
    assert(map.contains("cherry"));
    assert(!map.contains("fig"));

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

    custom::unordered_map<std::string, int, StringHash> map;
    map.insert({"apple", 1});
    map.insert({"banana", 2});
    map.insert({"cherry", 3});

    assert(map.size() == 3);
    assert(map["apple"] == 1);
    assert(map["banana"] == 2);
    assert(map["cherry"] == 3);

    std::cout << "Passed!" << std::endl;
}

void test_clear() {
    std::cout << "Testing clear... ";

    custom::unordered_map<std::string, int> map{
        {"apple", 1},
        {"banana", 2},
        {"cherry", 3}
    };

    assert(map.size() == 3);
    map.clear();
    assert(map.empty());
    assert(map.size() == 0);

    // Test that we can still insert after clearing
    map.insert({"date", 4});
    assert(map.size() == 1);
    assert(map["date"] == 4);

    std::cout << "Passed!" << std::endl;
}

int main() {
    std::cout << "Starting unordered_map tests...\n" << std::endl;

    test_constructors();
    test_insert_and_erase();
    test_element_access();
    test_iterators();
    test_find_and_contains();
    test_custom_hash();
    test_clear();

    std::cout << "\nAll unordered_map tests passed successfully!" << std::endl;
    return 0;
} 