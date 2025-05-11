#pragma once

#include "../../Sequential Containers/vector/vector.h" // Default underlying container
// Or #include "../../Sequential Containers/list/list.h"
// Or #include "../../Sequential Containers/deque/deque.h"
#include <utility> // For std::move, std::swap

namespace custom {

/**
 * @brief A container adapter that provides a stack (LIFO) interface.
 *
 * @tparam T Type of the elements.
 * @tparam Container The type of the underlying container, defaults to custom::vector<T>.
 */
template <typename T, typename Container = custom::vector<T>>
class stack {
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
    stack() : c() {}

    explicit stack(const Container& cont) : c(cont) {}
    explicit stack(Container&& cont) : c(std::move(cont)) {}

    stack(const stack& other) = default;
    stack(stack&& other) noexcept = default;

    stack& operator=(const stack& other) = default;
    stack& operator=(stack&& other) noexcept = default;

    // Element access
    reference top() { return c.back(); } // Relies on underlying container having back()
    const_reference top() const { return c.back(); }

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

    template <typename... Args>
    decltype(auto) emplace(Args&&... args) { // C++14 decltype(auto) for perfect return
        // Requires emplace_back on container. Our custom::vector may not have it yet.
        // Fallback to push_back(T(...))
#if defined(__cpp_lib_generic_emplace) && __cpp_lib_generic_emplace >= 201707L // A general check
        // if custom::vector has emplace_back: return c.emplace_back(std::forward<Args>(args)...);
        // For now, using push_back as vector might not have emplace_back.
        c.push_back(T(std::forward<Args>(args)...));
        // std::stack::emplace returns void in C++11/14, reference to new element in C++17
        // For simplicity and to match std::stack more closely, let's aim for void.
        // If we want to return a reference, it would be: return c.back();
#else
        c.push_back(T(std::forward<Args>(args)...));
#endif
        // Making it void to match std::stack pre-C++17 emplace return type behavior for adapters
    }

    void pop() {
        c.pop_back(); // Relies on underlying container having pop_back()
    }

    void swap(stack& other) noexcept(noexcept(c.swap(other.c))) {
        using std::swap;
        swap(c, other.c);
    }

    // Friend non-member functions for comparison
    template <typename T1, typename Container1>
    friend bool operator==(const stack<T1, Container1>& lhs, const stack<T1, Container1>& rhs);

    template <typename T1, typename Container1>
    friend bool operator!=(const stack<T1, Container1>& lhs, const stack<T1, Container1>& rhs);

    template <typename T1, typename Container1>
    friend bool operator<(const stack<T1, Container1>& lhs, const stack<T1, Container1>& rhs);

    template <typename T1, typename Container1>
    friend bool operator<=(const stack<T1, Container1>& lhs, const stack<T1, Container1>& rhs);

    template <typename T1, typename Container1>
    friend bool operator>(const stack<T1, Container1>& lhs, const stack<T1, Container1>& rhs);

    template <typename T1, typename Container1>
    friend bool operator>=(const stack<T1, Container1>& lhs, const stack<T1, Container1>& rhs);
};

// Non-member comparison functions
// These rely on the underlying container's comparison operators.
template <typename T, typename Container>
bool operator==(const stack<T, Container>& lhs, const stack<T, Container>& rhs) {
    return lhs.c == rhs.c;
}

template <typename T, typename Container>
bool operator!=(const stack<T, Container>& lhs, const stack<T, Container>& rhs) {
    return lhs.c != rhs.c;
}

template <typename T, typename Container>
bool operator<(const stack<T, Container>& lhs, const stack<T, Container>& rhs) {
    return lhs.c < rhs.c;
}

template <typename T, typename Container>
bool operator<=(const stack<T, Container>& lhs, const stack<T, Container>& rhs) {
    return lhs.c <= rhs.c;
}

template <typename T, typename Container>
bool operator>(const stack<T, Container>& lhs, const stack<T, Container>& rhs) {
    return lhs.c > rhs.c;
}

template <typename T, typename Container>
bool operator>=(const stack<T, Container>& lhs, const stack<T, Container>& rhs) {
    return lhs.c >= rhs.c;
}

// Non-member swap
template <typename T, typename Container>
void swap(stack<T, Container>& lhs, stack<T, Container>& rhs) noexcept(noexcept(lhs.swap(rhs))) {
    lhs.swap(rhs);
}

} // namespace custom 