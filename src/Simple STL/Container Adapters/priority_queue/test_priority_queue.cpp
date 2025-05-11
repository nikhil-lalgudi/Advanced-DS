#include "priority_queue.h"
#include "../../Sequential Containers/list/list.h" // For alternative container (though not ideal for PQ)
#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <algorithm> // For std::sort to verify order

// Helper to drain and print PQ sorted (for max-heap, prints in descending order)
template <typename PQ>
void print_pq_sorted(PQ& pq, const std::string& name) {
    std::cout << name << " (sorted drain): ";
    std::vector<typename PQ::value_type> items;
    while(!pq.empty()) {
        items.push_back(pq.top());
        std::cout << pq.top() << " ";
        pq.pop();
    }
    std::cout << std::endl;
    // Restore PQ for further tests if needed (by pushing items back)
    // For simplicity, this test function consumes the PQ.
}

void test_pq_default_max_heap() {
    std::cout << "\n--- Test Priority Queue (default custom::vector, std::less for max-heap) ---" << std::endl;
    custom::priority_queue<int> pq;

    assert(pq.empty());
    assert(pq.size() == 0);

    pq.push(30);
    assert(!pq.empty() && pq.size() == 1 && pq.top() == 30);
    std::cout << "Pushed 30. Top: " << pq.top() << ", Size: " << pq.size() << std::endl;

    pq.push(100);
    assert(pq.size() == 2 && pq.top() == 100);
    std::cout << "Pushed 100. Top: " << pq.top() << ", Size: " << pq.size() << std::endl;

    pq.push(20);
    assert(pq.size() == 3 && pq.top() == 100);
    std::cout << "Pushed 20. Top: " << pq.top() << ", Size: " << pq.size() << std::endl;

    pq.emplace(50); // Basic emplace
    assert(pq.size() == 4 && pq.top() == 100);
    std::cout << "Emplaced 50. Top: " << pq.top() << ", Size: " << pq.size() << std::endl;

    pq.push(120);
    assert(pq.size() == 5 && pq.top() == 120);
    std::cout << "Pushed 120. Top: " << pq.top() << ", Size: " << pq.size() << std::endl;

    std::cout << "Draining PQ (should be in descending order for max-heap):" << std::endl;
    std::vector<int> expected_order = {120, 100, 50, 30, 20};
    size_t i = 0;
    while(!pq.empty()) {
        assert(pq.top() == expected_order[i++]);
        std::cout << "Top: " << pq.top() << ", Popping..." << std::endl;
        pq.pop();
    }
    assert(i == expected_order.size());
    assert(pq.empty());

    // Test constructor with iterators
    std::vector<int> data = {5, 1, 9, 3, 7};
    custom::priority_queue<int> pq_from_iter(data.begin(), data.end());
    assert(pq_from_iter.size() == 5 && pq_from_iter.top() == 9);
    std::cout << "PQ from iterators {5,1,9,3,7}. Top: " << pq_from_iter.top() << std::endl;
    print_pq_sorted(pq_from_iter, "pq_from_iter");
}

void test_pq_min_heap() {
    std::cout << "\n--- Test Priority Queue (custom::vector, std::greater for min-heap) ---" << std::endl;
    custom::priority_queue<int, custom::vector<int>, std::greater<int>> min_pq;

    min_pq.push(30);
    min_pq.push(100);
    min_pq.push(20);
    min_pq.emplace(50);
    min_pq.push(10);

    assert(min_pq.top() == 10);
    std::cout << "Min-PQ initial top: " << min_pq.top() << std::endl;

    std::cout << "Draining Min-PQ (should be in ascending order for min-heap):" << std::endl;
    std::vector<int> expected_min_order = {10, 20, 30, 50, 100};
    size_t i = 0;
    while(!min_pq.empty()) {
        assert(min_pq.top() == expected_min_order[i++]);
        std::cout << "Top: " << min_pq.top() << ", Popping..." << std::endl;
        min_pq.pop();
    }
    assert(i == expected_min_order.size());
    assert(min_pq.empty());

    // Test constructor with iterators and custom comparator
    std::vector<int> data = {50, 15, 95, 35, 75};
    custom::priority_queue<int, custom::vector<int>, std::greater<int>> min_pq_iter(data.begin(), data.end());
    assert(min_pq_iter.size() == 5 && min_pq_iter.top() == 15);
    std::cout << "Min-PQ from iterators {50,15,95,35,75}. Top: " << min_pq_iter.top() << std::endl;
    print_pq_sorted(min_pq_iter, "min_pq_iter");

}

void test_pq_swap_and_copy_move() {
    std::cout << "\n--- Test Priority Queue Swap, Copy, Move ---" << std::endl;
    custom::priority_queue<std::string> pq1;
    pq1.push("banana"); pq1.push("apple"); pq1.push("cherry"); // apple, banana, cherry -> top: cherry

    custom::priority_queue<std::string> pq2;
    pq2.push("fig"); pq2.push("date"); // date, fig -> top: fig

    std::cout << "pq1 top before swap: " << pq1.top() << " size: " << pq1.size() << std::endl;
    std::cout << "pq2 top before swap: " << pq2.top() << " size: " << pq2.size() << std::endl;
    pq1.swap(pq2);
    assert(pq1.top() == "fig" && pq1.size() == 2);
    assert(pq2.top() == "cherry" && pq2.size() == 3);
    std::cout << "pq1 top after swap: " << pq1.top() << " size: " << pq1.size() << std::endl;
    std::cout << "pq2 top after swap: " << pq2.top() << " size: " << pq2.size() << std::endl;

    custom::swap(pq1, pq2); // Non-member swap
    assert(pq1.top() == "cherry" && pq1.size() == 3);
    assert(pq2.top() == "fig" && pq2.size() == 2);
    std::cout << "pq1 top after non-member swap: " << pq1.top() << " size: " << pq1.size() << std::endl;
    std::cout << "pq2 top after non-member swap: " << pq2.top() << " size: " << pq2.size() << std::endl;

    // Copy constructor
    custom::priority_queue<std::string> pq_copy(pq1);
    assert(pq_copy.size() == pq1.size());
    if (!pq1.empty()) assert(pq_copy.top() == pq1.top());

    // Move constructor
    custom::priority_queue<std::string> pq_move(std::move(pq1));
    assert(pq_move.size() == pq_copy.size());
    // pq1 is moved-from, its state is valid but unspecified (likely empty for vector)
    std::cout << "pq1 (after move construction from it) size: " << pq1.size() << std::endl;
    // assert(pq1.empty()); // This depends on vector's move constructor behavior

    // Copy assignment
    custom::priority_queue<std::string> pq_assign_copy;
    pq_assign_copy = pq_copy;
    assert(pq_assign_copy.size() == pq_copy.size());
    if (!pq_copy.empty()) assert(pq_assign_copy.top() == pq_copy.top());

    // Move assignment
    custom::priority_queue<std::string> pq_assign_move;
    pq_assign_move = std::move(pq_copy);
    assert(pq_assign_move.size() == pq_move.size());
    // pq_copy is moved-from
    std::cout << "pq_copy (after move assignment from it) size: " << pq_copy.size() << std::endl;
}

int main() {
    std::cout << "====== Running custom::priority_queue Tests ======" << std::endl;
    test_pq_default_max_heap();
    test_pq_min_heap();
    test_pq_swap_and_copy_move();
    std::cout << "\n====== custom::priority_queue Tests Passed! ======" << std::endl;
    return 0;
} 