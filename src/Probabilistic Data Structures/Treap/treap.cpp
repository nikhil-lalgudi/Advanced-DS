// Note this is just tester code

#include <iostream>
#include <cassert>
#include <vector>
#include <algorithm>
#include <random>
#include "treap.hpp"

void basic_functionality_test() {
    ds::Treap<int> treap;
    assert(treap.empty() == true);
    assert(treap.size() == 0);
    treap.insert(5);
    treap.insert(3);
    treap.insert(7);
    treap.insert(1);
    treap.insert(9);
    // basic check
    assert(treap.size() == 5);
    assert(treap.contains(5) == true);
    assert(treap.contains(3) == true);
    assert(treap.contains(10) == false);
    
    // order check
    assert(treap.order_of_key(1) == 0);
    assert(treap.order_of_key(3) == 1);
    assert(treap.order_of_key(5) == 2);
    assert(treap.order_of_key(7) == 3);
    assert(treap.order_of_key(9) == 4);
    // retrieval check
    assert(treap.find_kth(0).value() == 1);
    assert(treap.find_kth(2).value() == 5);
    assert(treap.find_kth(4).value() == 9);
    // erase 
    treap.erase(5);
    assert(treap.contains(5) == false);
    assert(treap.size() == 4);
    std::cout << "Basic functionality test passed!" << std::endl;
}

void stress_test() {
    std::cout << "Running stress test..." << std::endl;
    std::mt19937 rng(std::random_device{}());
    ds::Treap<int> treap;
    std::vector<int> reference_vec;
    for (int i = 0; i < 10000; ++i) {
        int val;
        do {
            val = std::uniform_int_distribution<int>(0, 1000000)(rng);
        } while (std::find(reference_vec.begin(), reference_vec.end(), val) != reference_vec.end());
        
        treap.insert(val);
        reference_vec.push_back(val);
    }
    std::sort(reference_vec.begin(), reference_vec.end());
    for (size_t i = 0; i < reference_vec.size(); ++i) {
        assert(treap.find_kth(i).value() == reference_vec[i]);
    }
    for (size_t i = 0; i < reference_vec.size(); ++i) {
        assert(treap.order_of_key(reference_vec[i]) == i);
    }
    std::shuffle(reference_vec.begin(), reference_vec.end(), rng);
    for (int val : reference_vec) {
        treap.erase(val);
        assert(treap.contains(val) == false);
    }
    assert(treap.empty() == true);
    std::cout << "Stress test passed!" << std::endl;
}

void custom_comparator_test() {
    std::cout << "Running custom comparator test..." << std::endl;
    ds::Treap<int, std::greater<int>> treap_desc;
    
    treap_desc.insert(5);
    treap_desc.insert(3);
    treap_desc.insert(7);
    treap_desc.insert(1);
    treap_desc.insert(9);
    assert(treap_desc.find_kth(0).value() == 9);
    assert(treap_desc.find_kth(2).value() == 5);
    assert(treap_desc.find_kth(4).value() == 1);
    
    std::cout << "Custom comparator test passed!" << std::endl;
}

int main() {
    try {
        basic_functionality_test();
        stress_test();
        custom_comparator_test();
        
        std::cout << "All Treap tests passed successfully!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}