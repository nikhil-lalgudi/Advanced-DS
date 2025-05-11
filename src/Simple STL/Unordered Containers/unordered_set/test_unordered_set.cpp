#include "unordered_set.h"
#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <algorithm> // For std::sort, std::find to verify contents
#include <set> // For comparing against std::set for content verification (order-independent)

// Helper to print unordered_set contents
template <typename USet>
void print_unordered_set(const USet& us, const std::string& name = "") {
    if (!name.empty()) {
        std::cout << name << ": ";
    }
    std::cout << "Size: " << us.size() 
              << ", Buckets: " << us.bucket_count()
              << ", Load Factor: " << us.load_factor()
              << ", Elements: { ";
    bool first = true;
    for (const auto& val : us) {
        if (!first) std::cout << ", ";
        std::cout << val;
        first = false;
    }
    std::cout << " }" << std::endl;
}

// Helper to check if two unordered_sets contain the same elements
// (ignoring order and bucket structure)
template <typename Key, typename Hash, typename KeyEqual, typename Alloc>
bool have_same_elements(const custom::unordered_set<Key, Hash, KeyEqual, Alloc>& cus,
                        const std::vector<Key>& expected_elements) {
    if (cus.size() != expected_elements.size()) return false;
    for (const auto& elem : expected_elements) {
        if (!cus.contains(elem)) return false;
    }
    // Also check if cus contains any extra elements (covered by size check already if implemented correctly)
    // but an explicit loop can be more robust for debugging
    for(const auto& elem_in_cus : cus) {
        bool found = false;
        for(const auto& expected_elem : expected_elements) {
            if (elem_in_cus == expected_elem) {
                found = true;
                break;
            }
        }
        if (!found) return false;
    }
    return true;
}

struct SimpleHash {
    std::size_t operator()(int k) const { return static_cast<std::size_t>(k); } // Simple identity hash for int
    std::size_t operator()(const std::string& s) const { 
        // Basic string hash
        std::size_t hash_val = 0;
        for (char ch : s) {
            hash_val = hash_val * 31 + ch;
        }
        return hash_val;
    }
};

struct SimpleKeyEqual {
    bool operator()(int k1, int k2) const { return k1 == k2; }
    bool operator()(const std::string& s1, const std::string& s2) const { return s1 == s2; }
};

void test_uset_constructors_and_assignment() {
    std::cout << "\n--- Test Unordered_Set Constructors and Assignment ---" << std::endl;

    // Default constructor
    custom::unordered_set<int> us1;
    assert(us1.empty());
    assert(us1.size() == 0);
    assert(us1.bucket_count() >= 1); // Should have some default buckets
    print_unordered_set(us1, "us1 (default)");

    // Constructor with bucket count
    custom::unordered_set<int> us2(50);
    assert(us2.empty() && us2.bucket_count() >= 50);
    print_unordered_set(us2, "us2 (bucket_count 50)");

    // Range constructor
    std::vector<int> ivec = {1, 2, 3, 1, 4, 5, 2};
    custom::unordered_set<int> us3(ivec.begin(), ivec.end());
    assert(us3.size() == 5); // Unique elements: 1, 2, 3, 4, 5
    assert(us3.contains(1) && us3.contains(2) && us3.contains(3) && us3.contains(4) && us3.contains(5));
    print_unordered_set(us3, "us3 (range from vector)");

    // Copy constructor
    custom::unordered_set<int> us4(us3);
    assert(us4.size() == us3.size());
    assert(have_same_elements(us4, {1,2,3,4,5}));
    print_unordered_set(us4, "us4 (copy of us3)");

    // Initializer list constructor
    custom::unordered_set<std::string, SimpleHash, SimpleKeyEqual> us5 = {"hello", "world", "!", "hello"};
    assert(us5.size() == 3);
    assert(us5.contains("hello") && us5.contains("world") && us5.contains("!"));
    print_unordered_set(us5, "us5 (initializer_list with custom hash/equal)");

    // Copy assignment
    us1 = us3;
    assert(us1.size() == us3.size());
    assert(have_same_elements(us1, {1,2,3,4,5}));
    print_unordered_set(us1, "us1 (assigned from us3)");

    // Move constructor
    custom::unordered_set<int> us6(std::move(us3));
    assert(us6.size() == 5);
    assert(us3.empty() || us3.size() == 0); // us3 should be empty or small after move
    assert(have_same_elements(us6, {1,2,3,4,5}));
    print_unordered_set(us6, "us6 (move from us3)");
    print_unordered_set(us3, "us3 (after move)");

    // Move assignment
    custom::unordered_set<int> us7;
    us7 = std::move(us6);
    assert(us7.size() == 5);
    assert(us6.empty() || us6.size() == 0);
    assert(have_same_elements(us7, {1,2,3,4,5}));
    print_unordered_set(us7, "us7 (move assigned from us6)");
    print_unordered_set(us6, "us6 (after move assignment)");
    
    custom::unordered_set<int> us8;
    us8 = {10, 20, 30, 10};
    assert(us8.size() == 3);
    assert(have_same_elements(us8, {10,20,30}));
    print_unordered_set(us8, "us8 (initializer list assignment)");
}

void test_uset_modifiers_and_lookup() {
    std::cout << "\n--- Test Unordered_Set Modifiers and Lookup ---" << std::endl;
    custom::unordered_set<int, SimpleHash> us;

    // insert
    auto p1 = us.insert(10);
    assert(p1.second && *(p1.first) == 10 && us.size() == 1 && us.contains(10));
    print_unordered_set(us, "Inserted 10");

    auto p2 = us.insert(20);
    assert(p2.second && *(p2.first) == 20 && us.size() == 2 && us.contains(20));
    print_unordered_set(us, "Inserted 20");

    auto p3 = us.insert(10); // Insert duplicate
    assert(!p3.second && *(p3.first) == 10 && us.size() == 2);
    print_unordered_set(us, "Inserted 10 again");

    // insert rvalue
    us.insert(30);
    assert(us.size() == 3 && us.contains(30));
    print_unordered_set(us, "Inserted 30 (rvalue)");

    // insert range
    std::vector<int> range_data = {40, 20, 50, 60, 40};
    us.insert(range_data.begin(), range_data.end());
    assert(us.size() == 6); // {10,20,30,40,50,60}
    assert(have_same_elements(us, {10,20,30,40,50,60}));
    print_unordered_set(us, "After insert range");

    // count and find
    assert(us.count(10) == 1);
    assert(us.count(100) == 0);
    assert(us.find(20) != us.end() && *(us.find(20)) == 20);
    assert(us.find(200) == us.end());
    assert(us.contains(50));
    assert(!us.contains(500));

    // erase by key
    assert(us.erase(30) == 1);
    assert(us.size() == 5 && !us.contains(30));
    print_unordered_set(us, "Erased 30");
    assert(us.erase(300) == 0); // Erase non-existent
    assert(us.size() == 5);

    // clear
    us.clear();
    assert(us.empty() && us.size() == 0);
    assert(us.bucket_count() > 0); // Buckets should remain
    print_unordered_set(us, "After clear");

    // swap
    custom::unordered_set<std::string> us_s1, us_s2;
    us_s1.insert("apple"); us_s1.insert("banana");
    us_s2.insert("cherry");
    print_unordered_set(us_s1, "us_s1 before swap");
    print_unordered_set(us_s2, "us_s2 before swap");
    us_s1.swap(us_s2);
    assert(us_s1.size()==1 && us_s1.contains("cherry"));
    assert(us_s2.size()==2 && us_s2.contains("apple") && us_s2.contains("banana"));
    print_unordered_set(us_s1, "us_s1 after swap");
    print_unordered_set(us_s2, "us_s2 after swap");
}

void test_uset_hash_policy() {
    std::cout << "\n--- Test Unordered_Set Hash Policy ---" << std::endl;
    custom::unordered_set<int> us(5); // Initial 5 buckets
    assert(us.bucket_count() >= 5);
    us.max_load_factor(0.5f);
    assert(us.max_load_factor() == 0.5f);

    us.insert(1); // size 1, buckets 5, LF = 0.2
    us.insert(2); // size 2, buckets 5, LF = 0.4
    print_unordered_set(us, "Before rehash potentially");
    size_t bc_before = us.bucket_count();
    us.insert(3); // size 3, buckets 5, LF = 0.6 -> should trigger rehash (3 / 0.5 = 6 buckets needed)
    print_unordered_set(us, "After inserting 3 (maybe rehashed)");
    assert(us.bucket_count() > bc_before || us.bucket_count() >= std::ceil(3 / 0.5f));
    assert(us.size() == 3 && us.contains(1) && us.contains(2) && us.contains(3));

    us.insert(4);
    us.insert(5);
    us.insert(6);
    us.insert(7);
    print_unordered_set(us, "After more inserts");

    // rehash
    size_t current_buckets = us.bucket_count();
    us.rehash(current_buckets * 3); // Request more buckets
    assert(us.bucket_count() >= current_buckets * 3 || us.bucket_count() >= std::ceil(us.size() / us.max_load_factor()));
    assert(us.size() == 7);
    assert(have_same_elements(us, {1,2,3,4,5,6,7}));
    print_unordered_set(us, "After rehash to more buckets");

    // reserve
    us.reserve(20); // Ensure capacity for 20 elements based on max_load_factor
    assert(us.bucket_count() >= std::ceil(20 / us.max_load_factor()));
    assert(us.size() == 7);
    print_unordered_set(us, "After reserve for 20 elements");
    
    // Test rehash to smaller number of buckets if elements allow
    custom::unordered_set<int> us_small(100);
    us_small.max_load_factor(1.0f);
    us_small.insert(1); us_small.insert(2);
    assert(us_small.bucket_count() >= 100);
    us_small.rehash(5); // num_elements is 2, 2/1.0 = 2. min buckets for rehash is 5.
    assert(us_small.bucket_count() >= 5); // Should be at least 5.
    assert(us_small.size() == 2);
    print_unordered_set(us_small, "us_small after rehash to 5");

}

void test_uset_iterators() {
    std::cout << "\n--- Test Unordered_Set Iterators (Simplified) ---" << std::endl;
    custom::unordered_set<int> us;
    us.insert(10); us.insert(20); us.insert(5); us.insert(15);
    print_unordered_set(us, "Set for iteration");

    std::vector<int> iterated_elements;
    for(const int& val : us) {
        iterated_elements.push_back(val);
        std::cout << "Iterated: " << val << std::endl;
    }
    assert(iterated_elements.size() == 4);
    // Check if all original elements are present (order doesn't matter for this check)
    assert(std::find(iterated_elements.begin(), iterated_elements.end(), 5) != iterated_elements.end());
    assert(std::find(iterated_elements.begin(), iterated_elements.end(), 10) != iterated_elements.end());
    assert(std::find(iterated_elements.begin(), iterated_elements.end(), 15) != iterated_elements.end());
    assert(std::find(iterated_elements.begin(), iterated_elements.end(), 20) != iterated_elements.end());

    custom::unordered_set<int> empty_us;
    int count = 0;
    for(const int& val : empty_us) {
        (void)val; // Supress unused warning
        count++;
    }
    assert(count == 0);
    assert(empty_us.begin() == empty_us.end());

    // Test find returns valid iterator
    auto it_found = us.find(10);
    assert(it_found != us.end() && *it_found == 10);
    auto it_not_found = us.find(100);
    assert(it_not_found == us.end());
}

int main() {
    std::cout << "====== Running custom::unordered_set Tests ======" << std::endl;
    test_uset_constructors_and_assignment();
    test_uset_modifiers_and_lookup();
    test_uset_hash_policy();
    test_uset_iterators();
    std::cout << "\n====== custom::unordered_set Tests Passed! ======" << std::endl;
    return 0;
} 