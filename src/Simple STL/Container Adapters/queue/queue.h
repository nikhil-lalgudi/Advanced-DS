#pragma once

#include "../../Sequential Containers/list/list.h" // Default underlying container
// Or #include "../../Sequential Containers/deque/deque.h" if available and preferred
#include <utility> // For std::move, std::swap

namespace custom {

/**
 * @brief A container adapter that provides a queue (FIFO) interface.
 *
 * @tparam T Type of the elements.
 * @tparam Container The type of the underlying container, defaults to custom::list<T>.
 */
template <typename T, typename Container = custom::list<T>>
class queue {
public:
    // Member types
    using container_type  = Container;
    using value_type      = typename Container::value_type;
    using size_type       = typename Container::size_type;
    using reference       = typename Container::reference;
    using const_reference = typename Container::const_reference;

protected:
    Container c; // Underlying container instance

public:
    // Constructors
    queue() : c() {}

    explicit queue(const Container& cont) : c(cont) {}
    explicit queue(Container&& cont) : c(std::move(cont)) {}

    // Prevent copy construction/assignment implicitly for adapters
    // (though std::queue allows copy if underlying container is copyable)
    // For this version, let's stick to simpler move/default semantics
    queue(const queue& other) = default;            // if Container is copyable
    queue(queue&& other) noexcept = default;    // if Container is movable

    queue& operator=(const queue& other) = default; // if Container is copy-assignable
    queue& operator=(queue&& other) noexcept = default; // if Container is move-assignable

    // Element access
    reference front() { return c.front(); }
    const_reference front() const { return c.front(); }

    reference back() { return c.back(); } // Relies on underlying container having back()
    const_reference back() const { return c.back(); }

    // Capacity
    bool empty() const { return c.empty(); }
    size_type size() const { return c.size(); }

    // Modifiers
    void push(const value_type& value) {
        c.push_back(value);
    }

    void push(value_type&& value) {
        c.push_back(std::move(value));
    }

    // emplace (C++11 style, basic version)
    template <typename... Args>
    decltype(auto) emplace(Args&&... args) { // C++14 decltype(auto) for perfect return
        c.emplace_back(std::forward<Args>(args)...); // Requires emplace_back on container
        // If container doesn't have emplace_back, this would be: 
        // c.push_back(T(std::forward<Args>(args)...));
        // For list, emplace_back is available in std::list. Assuming custom::list will have it.
        // If custom::list does not have emplace_back, this needs adjustment or that feature added to list.
        // Let's assume custom::list has emplace_back for now or use push_back as fallback.
#if defined(__cpp_lib_generic_emplace) && __cpp_lib_generic_emplace >= 201707L // Check if container has emplace_back
        //return c.emplace_back(std::forward<Args>(args)...); 
        // custom::list current impl does not have emplace_back. Using push_back:
        c.push_back(T(std::forward<Args>(args)...));
        return c.back(); // std::queue::emplace returns void in C++11/14, reference in C++17
                         // For simplicity, match void/value return behavior if possible or just void.
                         // std::queue emplace returns void. Let's make it void.
#else
        c.push_back(T(std::forward<Args>(args)...));
#endif
    }

    void pop() {
        c.pop_front();
    }

    void swap(queue& other) noexcept(noexcept(c.swap(other.c))) {
        using std::swap;
        swap(c, other.c);
    }

    // Friend non-member functions for comparison (if needed, based on underlying container)
    // For queue, comparisons are typically element-wise based on iterating through copies or exposing iterators.
    // std::queue defines relational operators based on the underlying container's operators.
    // We need to define them to work with our protected member `c`.

    template <typename T1, typename Container1>
    friend bool operator==(const queue<T1, Container1>& lhs, const queue<T1, Container1>& rhs);

    template <typename T1, typename Container1>
    friend bool operator!=(const queue<T1, Container1>& lhs, const queue<T1, Container1>& rhs);

    template <typename T1, typename Container1>
    friend bool operator<(const queue<T1, Container1>& lhs, const queue<T1, Container1>& rhs);

    template <typename T1, typename Container1>
    friend bool operator<=(const queue<T1, Container1>& lhs, const queue<T1, Container1>& rhs);

    template <typename T1, typename Container1>
    friend bool operator>(const queue<T1, Container1>& lhs, const queue<T1, Container1>& rhs);

    template <typename T1, typename Container1>
    friend bool operator>=(const queue<T1, Container1>& lhs, const queue<T1, Container1>& rhs);
};

// Non-member comparison functions
template <typename T, typename Container>
bool operator==(const queue<T, Container>& lhs, const queue<T, Container>& rhs) {
    return lhs.c == rhs.c;
}

template <typename T, typename Container>
bool operator!=(const queue<T, Container>& lhs, const queue<T, Container>& rhs) {
    return lhs.c != rhs.c;
}

template <typename T, typename Container>
bool operator<(const queue<T, Container>& lhs, const queue<T, Container>& rhs) {
    return lhs.c < rhs.c;
}

template <typename T, typename Container>
bool operator<=(const queue<T, Container>& lhs, const queue<T, Container>& rhs) {
    return lhs.c <= rhs.c;
}

template <typename T, typename Container>
bool operator>(const queue<T, Container>& lhs, const queue<T, Container>& rhs) {
    return lhs.c > rhs.c;
}

template <typename T, typename Container>
bool operator>=(const queue<T, Container>& lhs, const queue<T, Container>& rhs) {
    return lhs.c >= rhs.c;
}

// Non-member swap
template <typename T, typename Container>
void swap(queue<T, Container>& lhs, queue<T, Container>& rhs) noexcept(noexcept(lhs.swap(rhs))) {
    lhs.swap(rhs);
}

} // namespace custom 