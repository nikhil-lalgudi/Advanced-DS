#include "vector.h"
#include <iostream>
#include <vector> // For comparison with std::vector behavior and for initializing data
#include <string>
#include <cassert>
#include <stdexcept>
#include <numeric> // For std::iota

// Helper to print vector contents
template <typename T, typename Alloc>
void print_vector(const custom::vector<T, Alloc>& vec, const std::string& name = "") {
    if (!name.empty()) {
        std::cout << name << ": ";
    }
    std::cout << "Size: " << vec.size() << ", Capacity: " << vec.capacity() << ", Elements: { ";
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i] << (i == vec.size() - 1 ? " " : ", ");
    }
    std::cout << "}" << std::endl;
}

void test_constructors_and_assignment() {
    std::cout << "\n--- Test Vector Constructors and Assignment ---" << std::endl;

    // Default constructor
    custom::vector<int> v1;
    assert(v1.empty());
    assert(v1.size() == 0);
    assert(v1.capacity() == 0);
    print_vector(v1, "v1 (default)");

    // Fill constructor
    custom::vector<int> v2(5, 100);
    assert(!v2.empty());
    assert(v2.size() == 5);
    assert(v2.capacity() >= 5);
    for (int val : v2) {
        assert(val == 100);
    }
    print_vector(v2, "v2 (fill)");

    // Count constructor (default value)
    custom::vector<int> v2_default(3);
     assert(v2_default.size() == 3);
     for(int val : v2_default) assert(val == 0); // int() is 0
     print_vector(v2_default, "v2_default (count)");

    // Range constructor
    std::vector<int> std_vec = {1, 2, 3, 4, 5};
    custom::vector<int> v3(std_vec.begin(), std_vec.end());
    assert(v3.size() == 5);
    assert(v3.capacity() >= 5);
    for (size_t i = 0; i < v3.size(); ++i) {
        assert(v3[i] == std_vec[i]);
    }
    print_vector(v3, "v3 (range from std::vector)");

    // Copy constructor
    custom::vector<int> v4(v3);
    assert(v4.size() == v3.size());
    assert(v4.capacity() >= v3.size());
    for (size_t i = 0; i < v4.size(); ++i) {
        assert(v4[i] == v3[i]);
    }
    print_vector(v4, "v4 (copy of v3)");

    // Initializer list constructor
    custom::vector<std::string> v5 = {"hello", "world", "!"};
    assert(v5.size() == 3);
    assert(v5.capacity() >= 3);
    assert(v5[0] == "hello" && v5[1] == "world" && v5[2] == "!");
    print_vector(v5, "v5 (initializer_list)");

    // Copy assignment
    v1 = v2;
    assert(v1.size() == v2.size());
    assert(v1.capacity() >= v2.size());
    for (size_t i = 0; i < v1.size(); ++i) {
        assert(v1[i] == v2[i]);
    }
    print_vector(v1, "v1 (assigned from v2)");

    // Move constructor
    custom::vector<int> v6(std::move(v2));
    assert(v6.size() == 5);
    assert(v6.capacity() >= 5);
    assert(v2.empty()); // v2 should be empty after move
    assert(v2.capacity() == 0); // and capacity 0
    print_vector(v6, "v6 (move from v2)");
    print_vector(v2, "v2 (after move)");

    // Move assignment
    custom::vector<int> v7;
    v7 = std::move(v6); 
    assert(v7.size() == 5);
    assert(v7.capacity() >= 5);
    assert(v6.empty());
    assert(v6.capacity() == 0);
    print_vector(v7, "v7 (move assigned from v6)");
    print_vector(v6, "v6 (after move assignment)");
    
    custom::vector<int> v8;
    v8 = {10, 20, 30};
    assert(v8.size() == 3);
    print_vector(v8, "v8 (initializer list assignment)");

    // Assign methods
    custom::vector<int> v_assign;
    v_assign.assign(5, 99);
    assert(v_assign.size() == 5);
    for(int x : v_assign) assert(x == 99);
    print_vector(v_assign, "v_assign (assign fill)");
    
    std::vector<int> assign_src = {11, 22, 33};
    v_assign.assign(assign_src.begin(), assign_src.end());
    assert(v_assign.size() == 3);
    assert(v_assign[0] == 11 && v_assign[1] == 22 && v_assign[2] == 33);
    print_vector(v_assign, "v_assign (assign range)");
}

void test_element_access_and_capacity() {
    std::cout << "\n--- Test Vector Element Access and Capacity ---" << std::endl;
    custom::vector<int> v = {10, 20, 30, 40, 50};
    print_vector(v, "vector v");

    // operator[]
    assert(v[0] == 10);
    v[1] = 25;
    assert(v[1] == 25);
    print_vector(v, "v after v[1]=25");

    // at()
    assert(v.at(2) == 30);
    v.at(3) = 45;
    assert(v.at(3) == 45);
    bool at_throws = false;
    try {
        v.at(v.size()); // out of bounds
    } catch (const std::out_of_range&) {
        at_throws = true;
    }
    assert(at_throws);
    print_vector(v, "v after at(3)=45");

    // front() and back()
    assert(v.front() == 10);
    v.front() = 5;
    assert(v.front() == 5);
    assert(v.back() == 50);
    v.back() = 55;
    assert(v.back() == 55);
    print_vector(v, "v after front/back modified");

    // data()
    int* p_data = v.data();
    assert(p_data[0] == 5);
    p_data[0] = 1;
    assert(v.front() == 1);
    print_vector(v, "v after data() modified");

    // empty(), size(), capacity()
    custom::vector<int> empty_v;
    assert(empty_v.empty());
    assert(empty_v.size() == 0);
    assert(empty_v.capacity() == 0);

    assert(!v.empty());
    assert(v.size() == 5);
    size_t current_cap = v.capacity();
    assert(current_cap >= 5);

    // reserve()
    v.reserve(current_cap + 10);
    assert(v.capacity() >= current_cap + 10);
    assert(v.size() == 5); // size should not change
    assert(v[0] == 1 && v[4] == 55); // elements preserved
    print_vector(v, "v after reserve");
    v.reserve(current_cap); // reserving less than current capacity should do nothing
    assert(v.capacity() >= current_cap + 10); 

    // shrink_to_fit()
    v.push_back(60); v.push_back(70);
    v.pop_back();
    size_t size_before_shrink = v.size(); // 6
    v.reserve(size_before_shrink + 20); // make capacity larger than size
    assert(v.capacity() > size_before_shrink);
    print_vector(v, "v before shrink_to_fit");
    v.shrink_to_fit();
    print_vector(v, "v after shrink_to_fit");
    // Note: shrink_to_fit is a non-binding request. Capacity might be >= size.
    // For this simple implementation, it should be exactly size if reallocate(size) was called.
    // If size is 0, capacity might become 0 or 1 depending on reallocate logic.
    if (size_before_shrink > 0) {
        assert(v.capacity() == size_before_shrink);
    } else {
        assert(v.capacity() <= 1); // or 0, depends on reallocate for 0.
    }
    assert(v.size() == size_before_shrink);
    if(size_before_shrink > 0) assert(v.back() == 60);
}

void test_iterators() {
    std::cout << "\n--- Test Vector Iterators ---" << std::endl;
    custom::vector<int> v = {1, 2, 3, 4};
    print_vector(v, "vector for iterators");

    std::cout << "Forward iteration: ";
    int expected = 1;
    for (auto it = v.begin(); it != v.end(); ++it) {
        assert(*it == expected);
        *it += 10; // Modify through iterator
        assert(*it == expected + 10);
        expected++;
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    print_vector(v, "vector after forward modification");
    // v is now {11, 12, 13, 14}

    std::cout << "Const Forward iteration: ";
    expected = 11;
    for (auto it = v.cbegin(); it != v.cend(); ++it) {
        assert(*it == expected++);
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    std::cout << "Reverse iteration: ";
    expected = 14;
    for (auto it = v.rbegin(); it != v.rend(); ++it) {
        assert(*it == expected);
        *it -= 5; // Modify through reverse iterator
        assert(*it == expected - 5);
        expected--;
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    print_vector(v, "vector after reverse modification");
    // v is now {6, 7, 8, 9}

    std::cout << "Const Reverse iteration: ";
    expected = 9;
    for (auto it = v.crbegin(); it != v.crend(); ++it) {
        assert(*it == expected--);
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    custom::vector<int> empty_v;
    assert(empty_v.begin() == empty_v.end());
    assert(empty_v.rbegin() == empty_v.rend());
}

void test_modifiers() {
    std::cout << "\n--- Test Vector Modifiers ---" << std::endl;
    custom::vector<int> v;

    // push_back
    v.push_back(10);
    print_vector(v, "push_back(10)");
    assert(v.size() == 1 && v.back() == 10);
    v.push_back(20);
    print_vector(v, "push_back(20)");
    assert(v.size() == 2 && v.back() == 20 && v.front() == 10);
    
    // Test push_back causing reallocation
    custom::vector<int> v_realloc;
    v_realloc.reserve(2);
    v_realloc.push_back(1); v_realloc.push_back(2);
    size_t cap_before = v_realloc.capacity();
    v_realloc.push_back(3); // Should reallocate
    assert(v_realloc.capacity() > cap_before);
    assert(v_realloc.size() == 3 && v_realloc[2] == 3);
    print_vector(v_realloc, "v_realloc after push_back causing realloc");

    // pop_back
    v.pop_back();
    print_vector(v, "pop_back()");
    assert(v.size() == 1 && v.back() == 10);
    v.pop_back();
    print_vector(v, "pop_back() again");
    assert(v.empty());
    v.pop_back(); // pop_back on empty should be a no-op (or well-defined)
    assert(v.empty());

    // insert single element
    v = {10, 30, 40};
    auto it = v.begin() + 1; // Points to 30
    it = v.insert(it, 20);
    print_vector(v, "insert(it_to_30, 20)"); // {10, 20, 30, 40}
    assert(v.size() == 4 && *it == 20 && v[0]==10 && v[1]==20 && v[2]==30 && v[3]==40);

    it = v.insert(v.begin(), 5);
    print_vector(v, "insert(begin(), 5)"); // {5, 10, 20, 30, 40}
    assert(v.size() == 5 && *it == 5 && v.front() == 5);

    it = v.insert(v.end(), 50);
    print_vector(v, "insert(end(), 50)"); // {5, 10, 20, 30, 40, 50}
    assert(v.size() == 6 && *it == 50 && v.back() == 50);

    // insert multiple elements (fill)
    it = v.begin() + 2; // Points to 20
    it = v.insert(it, 3, 99);
    print_vector(v, "insert(it_to_20, 3, 99)"); // {5, 10, 99, 99, 99, 20, 30, 40, 50}
    assert(v.size() == 9 && *it == 99);

    // insert range
    std::vector<int> v_ins = {100, 200};
    it = v.end() -1; // points to 50
    it = v.insert(it, v_ins.begin(), v_ins.end()); // {..., 40, 100, 200, 50}
    print_vector(v, "insert(it_to_50, range)");
    assert(v.size() == 11 && *it == 100);
    assert(v[v.size()-1] == 50 && v[v.size()-2] == 200 && v[v.size()-3] == 100);

    // insert initializer_list
    it = v.begin() + 1; // points to 10
    it = v.insert(it, {0, 1});
    print_vector(v, "insert(it_to_10, {0,1})");
    assert(v.size() == 13 && *it == 0);

    // erase single element
    v = {0,1,2,3,4,5};
    it = v.erase(v.begin() + 2); // erase 2, it points to 3
    print_vector(v, "erase(it_to_2)"); // {0,1,3,4,5}
    assert(v.size() == 5 && *it == 3 && v[2] == 3);
    
    it = v.erase(v.begin()); // erase 0, it points to 1
    print_vector(v, "erase(begin())"); // {1,3,4,5}
    assert(v.size() == 4 && *it == 1 && v.front() == 1);

    it = v.erase(v.end() - 1); // erase 5, it points to end()
    print_vector(v, "erase(end()-1)"); // {1,3,4}
    assert(v.size() == 3 && it == v.end() && v.back() == 4);

    // erase range
    v = {0,1,2,3,4,5,6,7};
    auto first_to_erase = v.begin() + 2; // 2
    auto last_to_erase = v.begin() + 5;  // 5 (erase [2,3,4])
    it = v.erase(first_to_erase, last_to_erase);
    print_vector(v, "erase(range [2,3,4])"); // {0,1,5,6,7}
    assert(v.size() == 5 && *it == 5 && v[2] == 5);
    assert(v[0]==0 && v[1]==1 && v[2]==5 && v[3]==6 && v[4]==7);

    // clear
    v.clear();
    print_vector(v, "After clear");
    assert(v.empty() && v.size() == 0);
    assert(v.capacity() > 0); // Capacity typically not changed by clear

    // resize
    v.resize(5, 77);
    print_vector(v, "resize(5, 77)");
    assert(v.size() == 5 && v.capacity() >= 5);
    for(int x : v) assert(x == 77);

    v.resize(2);
    print_vector(v, "resize(2)");
    assert(v.size() == 2 && v.capacity() >= 5); // capacity not shrunk
    assert(v[0]==77 && v[1]==77);
    
    v.resize(4, 88);
    print_vector(v, "resize(4, 88) from smaller size"); // {77,77,88,88}
    assert(v.size() == 4);
    assert(v[0]==77 && v[1]==77 && v[2]==88 && v[3]==88);
    
    v.resize(0);
    print_vector(v, "resize(0)");
    assert(v.empty());

    // swap
    custom::vector<std::string> v_swap1 = {"a", "b"};
    custom::vector<std::string> v_swap2 = {"x", "y", "z"};
    size_t cap1_before = v_swap1.capacity(), cap2_before = v_swap2.capacity();
    print_vector(v_swap1, "v_swap1 before swap");
    print_vector(v_swap2, "v_swap2 before swap");
    v_swap1.swap(v_swap2);
    print_vector(v_swap1, "v_swap1 after swap");
    print_vector(v_swap2, "v_swap2 after swap");
    assert(v_swap1.size() == 3 && v_swap1[0] == "x");
    assert(v_swap2.size() == 2 && v_swap2[0] == "a");
    assert(v_swap1.capacity() == cap2_before);
    assert(v_swap2.capacity() == cap1_before);
    
    custom::swap(v_swap1, v_swap2); // non-member swap
    print_vector(v_swap1, "v_swap1 after non-member swap");
    print_vector(v_swap2, "v_swap2 after non-member swap");
    assert(v_swap1.size() == 2 && v_swap1[0] == "a");
    assert(v_swap2.size() == 3 && v_swap2[0] == "x");
}

void test_comparison_operators() {
    std::cout << "\n--- Test Vector Comparison Operators ---" << std::endl;
    custom::vector<int> v1 = {1, 2, 3};
    custom::vector<int> v2 = {1, 2, 3};
    custom::vector<int> v3 = {1, 2, 4};
    custom::vector<int> v4 = {1, 2};
    custom::vector<int> v5 = {1, 2, 3, 0};

    print_vector(v1, "v1");
    print_vector(v2, "v2");
    print_vector(v3, "v3");
    print_vector(v4, "v4");
    print_vector(v5, "v5");

    assert(v1 == v2);
    assert(!(v1 != v2));

    assert(v1 != v3);
    assert(v1 != v4);
    assert(v1 != v5);

    assert(v1 < v3);
    assert(v4 < v1);
    assert(v1 < v5);
    
    assert(v1 <= v2);
    assert(v1 <= v3);
    assert(v2 <= v1);
    assert(v4 <= v1);

    assert(v3 > v1);
    assert(v1 >= v2);
    assert(v3 >= v1);
    assert(v1 >= v4);
    assert(v5 > v1);
}

// Main function to run tests
int main() {
    std::cout << "====== Running custom::vector Tests ======" << std::endl;
    test_constructors_and_assignment();
    test_element_access_and_capacity();
    test_iterators();
    test_modifiers();
    test_comparison_operators();
    std::cout << "\n====== custom::vector Tests Passed! ======" << std::endl;
    return 0;
} 