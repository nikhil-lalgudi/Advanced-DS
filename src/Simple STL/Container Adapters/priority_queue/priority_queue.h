#pragma once

#include "../../Sequential Containers/vector/vector.h" // Default underlying container
#include <functional> // For std::less (default Compare)
#include <algorithm>  // For std::push_heap, std::pop_heap, std::make_heap
#include <utility>    // For std::move, std::swap

namespace custom {

/**
 * @brief A container adapter that provides a priority queue.
 *
 * By default, it's a max-heap (largest element has highest priority).
 * Uses std::less<T> as the default comparator for the heap algorithms.
 *
 * @tparam T Type of the elements.
 * @tparam Container The type of the underlying container, defaults to custom::vector<T>.
 * @tparam Compare The comparison function object type, defaults to std::less<typename Container::value_type>.
 *                 Note: std::priority_queue uses std::less for a max-heap because heap algorithms
 *                 (like std::push_heap) expect a "less than" comparison for a max-heap structure.
 */
template <typename T, 
          typename Container = custom::vector<T>,
          typename Compare = std::less<typename Container::value_type>>
class priority_queue {
public:
    // Member types
    using container_type  = Container;
    using value_type      = typename Container::value_type;
    using size_type       = typename Container::size_type;
    using reference       = typename Container::reference; // Not directly exposed by std::priority_queue
    using const_reference = typename Container::const_reference;

protected:
    Container c;    // Underlying container
    Compare comp;   // Comparison function object

public:
    // Constructors
    priority_queue() : c(), comp() {}

    explicit priority_queue(const Compare& compare) : c(), comp(compare) {}

    priority_queue(const Compare& compare, const Container& cont)
        : c(cont), comp(compare) {
        std::make_heap(c.begin(), c.end(), comp);
    }

    priority_queue(const Compare& compare, Container&& cont)
        : c(std::move(cont)), comp(compare) {
        std::make_heap(c.begin(), c.end(), comp);
    }

    // Constructor from iterators
    template <typename InputIt>
    priority_queue(InputIt first, InputIt last,
                   const Compare& compare = Compare(),
                   const Container& cont = Container())
        : c(cont), comp(compare) {
        c.insert(c.end(), first, last); // Can also initialize c(first, last) if container supports it
        std::make_heap(c.begin(), c.end(), comp);
    }
    
    template <typename InputIt>
    priority_queue(InputIt first, InputIt last,
                   const Compare& compare,
                   Container&& cont)
        : c(std::move(cont)), comp(compare) {
        c.insert(c.end(), first, last); 
        std::make_heap(c.begin(), c.end(), comp);
    }

    // Defaulted copy/move constructors/assignment (if Container and Compare are copyable/movable)
    priority_queue(const priority_queue& other) = default;
    priority_queue(priority_queue&& other) noexcept = default;
    priority_queue& operator=(const priority_queue& other) = default;
    priority_queue& operator=(priority_queue&& other) noexcept = default;

    // Element access
    // std::priority_queue only exposes const_reference top()
    const_reference top() const {
        return c.front(); // Heap property ensures front is the highest priority
    }

    // Capacity
    bool empty() const { return c.empty(); }
    size_type size() const { return c.size(); }

    // Modifiers
    void push(const value_type& value) {
        c.push_back(value);
        std::push_heap(c.begin(), c.end(), comp);
    }

    void push(value_type&& value) {
        c.push_back(std::move(value));
        std::push_heap(c.begin(), c.end(), comp);
    }

    template <typename... Args>
    void emplace(Args&&... args) {
        // Requires emplace_back on container. Our custom::vector might not have it.
        // Fallback to push_back(T(...))
        c.push_back(T(std::forward<Args>(args)...)); 
        // Or if vector has emplace_back: c.emplace_back(std::forward<Args>(args)...);
        std::push_heap(c.begin(), c.end(), comp);
    }

    void pop() {
        std::pop_heap(c.begin(), c.end(), comp); // Moves the highest priority to the end
        c.pop_back();                             // Removes it from the container
    }

    void swap(priority_queue& other) noexcept(noexcept(c.swap(other.c)) && noexcept(std::swap(comp, other.comp))) {
        using std::swap;
        swap(c, other.c);
        swap(comp, other.comp);
    }
    
    // No direct comparison operators for priority_queue itself usually.
    // Equality would mean same elements in same heap order, which is complex.
    // Order comparisons don't make sense for a PQ directly.
};

// Non-member swap
template <typename T, typename Container, typename Compare>
void swap(priority_queue<T, Container, Compare>& lhs,
          priority_queue<T, Container, Compare>& rhs) noexcept(noexcept(lhs.swap(rhs))) {
    lhs.swap(rhs);
}

} // namespace custom 