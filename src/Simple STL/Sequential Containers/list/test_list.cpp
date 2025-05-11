#include "list.h"
#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <stdexcept>

// Helper to print list contents
template <typename T>
void print_list(const custom::list<T>& l, const std::string& name = "") {
    if (!name.empty()) {
        std::cout << name << ": ";
    }
    for (const auto& val : l) {
        std::cout << val << " ";
    }
    std::cout << "(size: " << l.size() << ")" << std::endl;
}

void test_constructors_and_assignment() {
    std::cout << "\n--- Test Constructors and Assignment ---" << std::endl;

    // Default constructor
    custom::list<int> l1;
    assert(l1.empty());
    assert(l1.size() == 0);
    print_list(l1, "l1 (default)");

    // Fill constructor
    custom::list<int> l2(5, 100);
    assert(!l2.empty());
    assert(l2.size() == 5);
    for (int val : l2) {
        assert(val == 100);
    }
    print_list(l2, "l2 (fill)");

    // Range constructor
    std::vector<int> vec = {1, 2, 3, 4, 5};
    custom::list<int> l3(vec.begin(), vec.end());
    assert(l3.size() == 5);
    int expected_val = 1;
    for (int val : l3) {
        assert(val == expected_val++);
    }
    print_list(l3, "l3 (range from vector)");

    // Copy constructor
    custom::list<int> l4(l3);
    assert(l4.size() == 5);
    assert(l3.size() == 5); // l3 should be unchanged
    expected_val = 1;
    for (int val : l4) {
        assert(val == expected_val++);
    }
    print_list(l4, "l4 (copy of l3)");

    // Initializer list constructor
    custom::list<std::string> l5 = {"hello", "world", "!"};
    assert(l5.size() == 3);
    print_list(l5, "l5 (initializer_list)");

    // Copy assignment
    l1 = l2;
    assert(l1.size() == l2.size());
    print_list(l1, "l1 (assigned from l2)");

    // Move constructor
    custom::list<int> l6(std::move(l2));
    assert(l6.size() == 5);
    assert(l2.empty()); // l2 should be empty after move
    assert(l2.size() == 0);
    print_list(l6, "l6 (move from l2)");
    print_list(l2, "l2 (after move)");

    // Move assignment
    l1 = custom::list<int>(3, 77); // Create a temporary to move from
    assert(l1.size() == 3);
    print_list(l1, "l1 (move assigned)");
    
    custom::list<int> l7;
    l7 = {10, 20, 30};
    assert(l7.size() == 3);
    print_list(l7, "l7 (initializer list assignment)");
}

void test_element_access() {
    std::cout << "\n--- Test Element Access ---" << std::endl;
    custom::list<int> l = {10, 20, 30};
    print_list(l, "list for element access");

    assert(l.front() == 10);
    l.front() = 15;
    assert(l.front() == 15);
    print_list(l, "list after front() modified");

    assert(l.back() == 30);
    l.back() = 35;
    assert(l.back() == 35);
    print_list(l, "list after back() modified");

    const custom::list<int> cl = {1, 2, 3};
    assert(cl.front() == 1);
    assert(cl.back() == 3);

    custom::list<int> empty_l;
    bool front_throws = false;
    try {
        empty_l.front();
    } catch (const std::out_of_range&) {
        front_throws = true;
    }
    assert(front_throws);

    bool back_throws = false;
    try {
        empty_l.back();
    } catch (const std::out_of_range&) {
        back_throws = true;
    }
    assert(back_throws);
}

void test_iterators() {
    std::cout << "\n--- Test Iterators ---" << std::endl;
    custom::list<int> l = {1, 2, 3, 4};
    print_list(l, "list for iterators");

    std::cout << "Forward iteration: ";
    int expected = 1;
    for (auto it = l.begin(); it != l.end(); ++it) {
        assert(*it == expected++);
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    std::cout << "Const Forward iteration: ";
    expected = 1;
    for (auto it = l.cbegin(); it != l.cend(); ++it) {
        assert(*it == expected++);
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    std::cout << "Reverse iteration: ";
    expected = 4;
    for (auto it = l.rbegin(); it != l.rend(); ++it) {
        assert(*it == expected--);
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    std::cout << "Const Reverse iteration: ";
    expected = 4;
    for (auto it = l.crbegin(); it != l.crend(); ++it) {
        assert(*it == expected--);
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    custom::list<int> empty_l;
    assert(empty_l.begin() == empty_l.end());
    assert(empty_l.rbegin() == empty_l.rend());
}

void test_modifiers() {
    std::cout << "\n--- Test Modifiers ---" << std::endl;
    custom::list<int> l;

    // push_front
    l.push_front(10);
    print_list(l, "push_front(10)");
    assert(l.size() == 1 && l.front() == 10 && l.back() == 10);
    l.push_front(5);
    print_list(l, "push_front(5)");
    assert(l.size() == 2 && l.front() == 5 && l.back() == 10);

    // push_back
    l.push_back(20);
    print_list(l, "push_back(20)");
    assert(l.size() == 3 && l.front() == 5 && l.back() == 20);
    l.push_back(25);
    print_list(l, "push_back(25)");
    assert(l.size() == 4 && l.front() == 5 && l.back() == 25);
    // l is now {5, 10, 20, 25}

    // pop_front
    l.pop_front();
    print_list(l, "pop_front()");
    assert(l.size() == 3 && l.front() == 10 && l.back() == 25);

    // pop_back
    l.pop_back();
    print_list(l, "pop_back()");
    assert(l.size() == 2 && l.front() == 10 && l.back() == 20);
    // l is now {10, 20}

    // insert single element
    auto it = l.begin(); // iterator to 10
    ++it; // iterator to 20
    l.insert(it, 15); // insert 15 before 20
    print_list(l, "insert(it_to_20, 15)"); // {10, 15, 20}
    assert(l.size() == 3);
    assert(*l.begin() == 10 && *(++l.begin()) == 15 && *(++(++l.begin())) == 20);
    
    it = l.begin();
    l.insert(it, 7); // insert 7 at beginning {7, 10, 15, 20}
    print_list(l, "insert(begin(), 7)");
    assert(l.size() == 4 && l.front() == 7);

    l.insert(l.end(), 30); // insert 30 at end {7, 10, 15, 20, 30}
    print_list(l, "insert(end(), 30)");
    assert(l.size() == 5 && l.back() == 30);

    // insert multiple elements (fill)
    it = ++l.begin(); // iterator to 10
    l.insert(it, 2, 99); // {7, 99, 99, 10, 15, 20, 30}
    print_list(l, "insert(it_to_10, 2, 99)");
    assert(l.size() == 7);

    // insert range
    std::vector<int> v_ins = {100, 200};
    it = l.end();
    l.insert(it, v_ins.begin(), v_ins.end());
    print_list(l, "insert(end(), range {100, 200})");
    assert(l.size() == 9 && l.back() == 200);

    // insert initializer_list
    it = l.begin();
    ++it; // iterator to 99
    l.insert(it, {50, 60});
    print_list(l, "insert(it_to_99, {50, 60})");
    assert(l.size() == 11);

    // erase single element
    it = l.begin(); // 7
    ++it; // 50
    it = l.erase(it); // erase 50, it now points to 60
    assert(*it == 60);
    print_list(l, "erase(it_to_50)");
    assert(l.size() == 10);
    
    it = l.erase(l.begin()); // erase front (7)
    assert(*it == 99); // it points to new front
    print_list(l, "erase(begin())");
    assert(l.size() == 9 && l.front() == 99);

    // erase range
    auto first_to_erase = ++l.begin(); // second 99
    auto last_to_erase = first_to_erase;
    ++last_to_erase; // 10
    ++last_to_erase; // 15
    it = l.erase(first_to_erase, last_to_erase); // erase [99, 10), it points to 15
    assert(*it == 15);
    print_list(l, "erase(range [99,10) )"); // list should be {99, 15, 20, 30, 100, 200}
    assert(l.size() == 6);

    // resize
    l.resize(8, 1); // {99, 15, 20, 30, 100, 200, 1, 1}
    print_list(l, "resize(8, 1)");
    assert(l.size() == 8);
    assert(l.back() == 1);

    l.resize(4); // {99, 15, 20, 30}
    print_list(l, "resize(4)");
    assert(l.size() == 4);
    assert(l.back() == 30);
    
    l.resize(0);
    print_list(l, "resize(0)");
    assert(l.empty());

    // clear
    l.push_back(1); l.push_back(2);
    print_list(l, "Before clear");
    l.clear();
    print_list(l, "After clear");
    assert(l.empty() && l.size() == 0);
    assert(l.begin() == l.end());

    // swap
    custom::list<int> l_swap1 = {1, 2, 3};
    custom::list<int> l_swap2 = {10, 20};
    print_list(l_swap1, "l_swap1 before swap");
    print_list(l_swap2, "l_swap2 before swap");
    l_swap1.swap(l_swap2);
    print_list(l_swap1, "l_swap1 after swap");
    print_list(l_swap2, "l_swap2 after swap");
    assert(l_swap1.size() == 2 && l_swap1.front() == 10);
    assert(l_swap2.size() == 3 && l_swap2.front() == 1);
    
    custom::swap(l_swap1, l_swap2); // non-member swap
    print_list(l_swap1, "l_swap1 after non-member swap");
    print_list(l_swap2, "l_swap2 after non-member swap");
    assert(l_swap1.size() == 3 && l_swap1.front() == 1);
    assert(l_swap2.size() == 2 && l_swap2.front() == 10);
}

void test_comparison_operators() {
    std::cout << "\n--- Test Comparison Operators ---" << std::endl;
    custom::list<int> l1 = {1, 2, 3};
    custom::list<int> l2 = {1, 2, 3};
    custom::list<int> l3 = {1, 2, 4};
    custom::list<int> l4 = {1, 2};

    print_list(l1, "l1");
    print_list(l2, "l2");
    print_list(l3, "l3");
    print_list(l4, "l4");

    assert(l1 == l2);
    assert(!(l1 != l2));

    assert(l1 != l3);
    assert(!(l1 == l3));

    assert(l1 != l4);
    assert(l4 != l1);

    assert(l1 < l3);
    assert(!(l3 < l1));
    assert(l4 < l1);
    assert(l4 < l3);

    assert(l1 <= l2);
    assert(l1 <= l3);
    assert(l2 <= l1);

    assert(l3 > l1);
    assert(l1 >= l2);
    assert(l3 >= l1);
}

// Main function to run tests
int main() {
    std::cout << "====== Running custom::list Tests ======" << std::endl;
    test_constructors_and_assignment();
    test_element_access();
    test_iterators();
    test_modifiers();
    test_comparison_operators();
    std::cout << "\n====== custom::list Tests Passed! ======" << std::endl;
    return 0;
} 