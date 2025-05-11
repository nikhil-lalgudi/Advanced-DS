#include "stack.h"
#include "../../Sequential Containers/list/list.h" // For an alternative container
#include <iostream>
#include <string>
#include <cassert>
#include <vector> // For std::vector as one of the possible underlying containers

void test_stack_default_container() {
    std::cout << "\n--- Test Stack (default custom::vector) ---" << std::endl;
    custom::stack<int> s;

    assert(s.empty());
    assert(s.size() == 0);

    s.push(10);
    assert(!s.empty());
    assert(s.size() == 1);
    assert(s.top() == 10);
    std::cout << "Pushed 10. Top: " << s.top() << ", Size: " << s.size() << std::endl;

    s.push(20);
    assert(s.size() == 2);
    assert(s.top() == 20);
    std::cout << "Pushed 20. Top: " << s.top() << ", Size: " << s.size() << std::endl;

    s.push(30);
    assert(s.size() == 3);
    assert(s.top() == 30);
    std::cout << "Pushed 30. Top: " << s.top() << ", Size: " << s.size() << std::endl;

    // Test emplace (basic version)
    s.emplace(40); // Assumes vector will get emplace_back or uses push_back(T(...))
    assert(s.size() == 4);
    assert(s.top() == 40);
    std::cout << "Emplaced 40. Top: " << s.top() << ", Size: " << s.size() << std::endl;

    assert(s.top() == 40);
    s.pop();
    assert(s.size() == 3);
    assert(s.top() == 30);
    std::cout << "Popped. Top: " << s.top() << ", Size: " << s.size() << std::endl;

    s.top() = 35; // Modify top
    assert(s.top() == 35);
    std::cout << "Modified top to 35. Top: " << s.top() << std::endl;

    custom::stack<int> s2;
    s2.push(100);
    s2.push(200);

    std::cout << "s before swap - Size: " << s.size() << ", Top: " << s.top() << std::endl;
    std::cout << "s2 before swap - Size: " << s2.size() << ", Top: " << s2.top() << std::endl;
    s.swap(s2);
    assert(s.size() == 2 && s.top() == 200);
    assert(s2.size() == 3 && s2.top() == 35);
    std::cout << "s after swap - Size: " << s.size() << ", Top: " << s.top() << std::endl;
    std::cout << "s2 after swap - Size: " << s2.size() << ", Top: " << s2.top() << std::endl;

    custom::swap(s, s2); // Non-member swap
    assert(s.size() == 3 && s.top() == 35);
    assert(s2.size() == 2 && s2.top() == 200);
    std::cout << "s after non-member swap - Size: " << s.size() << ", Top: " << s.top() << std::endl;
    std::cout << "s2 after non-member swap - Size: " << s2.size() << ", Top: " << s2.top() << std::endl;

    // Test copy and move semantics
    custom::stack<int> s_copy(s);
    assert(s_copy.size() == s.size());
    if(!s.empty() && !s_copy.empty()) assert(s_copy.top() == s.top());

    custom::stack<int> s_move(std::move(s));
    assert(s_move.size() == s_copy.size());
    // s is moved-from
    std::cout << "s (after move construction from it) size: " << s.size() << std::endl;

    custom::stack<int> s_assign_copy;
    s_assign_copy = s_copy;
    assert(s_assign_copy.size() == s_copy.size());
    if(!s_copy.empty() && !s_assign_copy.empty()) assert(s_assign_copy.top() == s_copy.top());

    custom::stack<int> s_assign_move;
    s_assign_move = std::move(s_copy);
    assert(s_assign_move.size() == s_move.size());
    std::cout << "s_copy (after move assignment from it) size: " << s_copy.size() << std::endl;
}

void test_stack_list_container() {
    std::cout << "\n--- Test Stack (custom::list as container) ---" << std::endl;
    custom::stack<std::string, custom::list<std::string>> s_list;
    assert(s_list.empty());
    s_list.push("alpha");
    s_list.push("beta");
    assert(s_list.size() == 2 && s_list.top() == "beta");
    std::cout << "Pushed alpha, beta. Top: " << s_list.top() << ", Size: " << s_list.size() << std::endl;
    s_list.pop();
    assert(s_list.size() == 1 && s_list.top() == "alpha");
    std::cout << "Popped. Top: " << s_list.top() << ", Size: " << s_list.size() << std::endl;
}

void test_stack_comparisons() {
    std::cout << "\n--- Test Stack Comparisons ---" << std::endl;
    custom::stack<int> s1, s2, s3, s4;
    s1.push(10); s1.push(20); // s1: {10, 20} (top is 20)
    s2.push(10); s2.push(20); // s2: {10, 20}
    s3.push(10); s3.push(30); // s3: {10, 30}
    s4.push(10);             // s4: {10}

    std::cout << "s1 (underlying vector): {10, 20}" << std::endl;
    std::cout << "s2 (underlying vector): {10, 20}" << std::endl;
    std::cout << "s3 (underlying vector): {10, 30}" << std::endl;
    std::cout << "s4 (underlying vector): {10}" << std::endl;

    assert(s1 == s2);
    assert(!(s1 != s2));

    assert(s1 != s3);
    assert(s1 != s4);

    // Based on underlying vector comparison
    assert(s1 < s3);      // {10,20} < {10,30}
    assert(s4 < s1);      // {10} < {10,20}
    
    assert(s1 <= s2);
    assert(s1 <= s3);
    assert(s4 <= s1);

    assert(s3 > s1);
    assert(s1 >= s2);
    assert(s3 >= s1);
    assert(s1 >= s4);
}

int main() {
    std::cout << "====== Running custom::stack Tests ======" << std::endl;
    test_stack_default_container();
    test_stack_list_container();
    test_stack_comparisons();
    std::cout << "\n====== custom::stack Tests Passed! ======" << std::endl;
    return 0;
} 