#include "queue.h"
#include "../../Sequential Containers/vector/vector.h" // For an alternative container
#include <iostream>
#include <string>
#include <cassert>
#include <vector> // For std::vector to compare behavior for some tests

void test_queue_default_container() {
    std::cout << "\n--- Test Queue (default custom::list) ---" << std::endl;
    custom::queue<int> q;

    assert(q.empty());
    assert(q.size() == 0);

    q.push(10);
    assert(!q.empty());
    assert(q.size() == 1);
    assert(q.front() == 10);
    assert(q.back() == 10);
    std::cout << "Pushed 10. Front: " << q.front() << ", Back: " << q.back() << ", Size: " << q.size() << std::endl;

    q.push(20);
    assert(q.size() == 2);
    assert(q.front() == 10);
    assert(q.back() == 20);
    std::cout << "Pushed 20. Front: " << q.front() << ", Back: " << q.back() << ", Size: " << q.size() << std::endl;

    q.push(30);
    assert(q.size() == 3);
    assert(q.front() == 10);
    assert(q.back() == 30);
    std::cout << "Pushed 30. Front: " << q.front() << ", Back: " << q.back() << ", Size: " << q.size() << std::endl;

    // Test emplace (basic version)
    q.emplace(40); // Assumes list will get emplace_back or uses push_back(T(...))
    assert(q.size() == 4);
    assert(q.front() == 10);
    assert(q.back() == 40);
    std::cout << "Emplaced 40. Front: " << q.front() << ", Back: " << q.back() << ", Size: " << q.size() << std::endl;

    assert(q.front() == 10);
    q.pop();
    assert(q.size() == 3);
    assert(q.front() == 20);
    assert(q.back() == 40);
    std::cout << "Popped. Front: " << q.front() << ", Back: " << q.back() << ", Size: " << q.size() << std::endl;

    q.front() = 25; // Modify front
    assert(q.front() == 25);
    std::cout << "Modified front to 25. Front: " << q.front() << std::endl;

    q.back() = 45; // Modify back
    assert(q.back() == 45);
    std::cout << "Modified back to 45. Back: " << q.back() << std::endl;

    custom::queue<int> q2;
    q2.push(100);
    q2.push(200);

    std::cout << "q before swap - Size: " << q.size() << ", Front: " << q.front() << std::endl;
    std::cout << "q2 before swap - Size: " << q2.size() << ", Front: " << q2.front() << std::endl;
    q.swap(q2);
    assert(q.size() == 2 && q.front() == 100);
    assert(q2.size() == 3 && q2.front() == 25);
    std::cout << "q after swap - Size: " << q.size() << ", Front: " << q.front() << std::endl;
    std::cout << "q2 after swap - Size: " << q2.size() << ", Front: " << q2.front() << std::endl;
    
    custom::swap(q, q2); // Non-member swap
    assert(q.size() == 3 && q.front() == 25);
    assert(q2.size() == 2 && q2.front() == 100);
    std::cout << "q after non-member swap - Size: " << q.size() << ", Front: " << q.front() << std::endl;
    std::cout << "q2 after non-member swap - Size: " << q2.size() << ", Front: " << q2.front() << std::endl;

    // Test copy and move semantics (if underlying container supports them)
    custom::queue<int> q_copy(q);
    assert(q_copy.size() == q.size());
    if(!q.empty() && !q_copy.empty()) assert(q_copy.front() == q.front() && q_copy.back() == q.back());

    custom::queue<int> q_move(std::move(q));
    assert(q_move.size() == q_copy.size()); 
    // q is now in a valid but unspecified state (moved-from for list)
    // For list, it should be empty ideally.
    // assert(q.empty()); // This depends on the list's move constructor behavior
     std::cout << "q (after move construction from it) size: " << q.size() << std::endl;

    custom::queue<int> q_assign_copy;
    q_assign_copy = q_copy;
    assert(q_assign_copy.size() == q_copy.size());
    if(!q_copy.empty() && !q_assign_copy.empty()) assert(q_assign_copy.front() == q_copy.front());

    custom::queue<int> q_assign_move;
    q_assign_move = std::move(q_copy);
    assert(q_assign_move.size() == q_move.size());
    // q_copy is moved-from
    // assert(q_copy.empty()); // Depends on list's move assignment behavior
    std::cout << "q_copy (after move assignment from it) size: " << q_copy.size() << std::endl;
}

void test_queue_custom_container() {
    std::cout << "\n--- Test Queue (custom::vector as container) ---" << std::endl;
    // Note: custom::vector is not ideal for queue (pop_front is inefficient)
    // but can be used to test adapter flexibility.
    // We would need custom::vector to have push_back, pop_front, front, back.
    // Our custom::vector has push_back, front, back. It does not have pop_front.
    // So custom::list is a better default. Let's use a std::vector for this example
    // or a hypothetical custom::deque if we had one.
    // For now, let's just show the structure, actual compilation would fail
    // if custom::vector doesn't satisfy all requirements. 
    // The current custom::vector has push_back, pop_back, front, back. queue needs pop_front.
    // Let's use std::deque which is a common choice for std::queue. 
    // However, the prompt is for *custom* containers.
    // Our custom::list works, so we'll stick to testing with that mostly.
    // This section is more to demonstrate the template nature.

    // To make this runnable, we'd need a custom::deque or ensure custom::vector supports pop_front().
    // For now, let's assume we are testing the default custom::list based custom::queue.
    // If we used custom::vector, it would fail because vector doesn't have pop_front().
    // custom::queue<std::string, custom::vector<std::string>> q_vec;
    // q_vec.push("test_vec");
    // if (!q_vec.empty()) {
    //     std::cout << "Queue with custom::vector front: " << q_vec.front() << std::endl;
    //     q_vec.pop(); // This would fail with current custom::vector
    // }
    std::cout << "(Skipping custom::vector as underlying container test due to missing pop_front in custom::vector)" << std::endl;
    std::cout << "(The default custom::list is suitable and tested above)" << std::endl;

}

void test_queue_comparisons() {
    std::cout << "\n--- Test Queue Comparisons ---" << std::endl;
    custom::queue<int> q1, q2, q3, q4;
    q1.push(10); q1.push(20);
    q2.push(10); q2.push(20);
    q3.push(10); q3.push(30);
    q4.push(10);

    std::cout << "q1: {10, 20}" << std::endl;
    std::cout << "q2: {10, 20}" << std::endl;
    std::cout << "q3: {10, 30}" << std::endl;
    std::cout << "q4: {10}" << std::endl;

    assert(q1 == q2);
    assert(!(q1 != q2));

    assert(q1 != q3);
    assert(q1 != q4);

    // Underlying list comparison defines <, <=, >, >=
    assert(q1 < q3);      // {10,20} < {10,30}
    assert(q4 < q1);      // {10} < {10,20}
    
    assert(q1 <= q2);
    assert(q1 <= q3);
    assert(q4 <= q1);

    assert(q3 > q1);
    assert(q1 >= q2);
    assert(q3 >= q1);
    assert(q1 >= q4);
}


int main() {
    std::cout << "====== Running custom::queue Tests ======" << std::endl;
    test_queue_default_container();
    test_queue_custom_container();
    test_queue_comparisons();
    std::cout << "\n====== custom::queue Tests Passed! ======" << std::endl;
    return 0;
} 