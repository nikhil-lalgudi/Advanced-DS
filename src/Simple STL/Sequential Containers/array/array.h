#pragma once

#include <algorithm>
#include <cstddef>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <iterator>
#include <array>

namespace custom {

/**
 * @brief A fixed-size container that encapsulates arrays.
 * 
 * @tparam T Type of the elements
 * @tparam N Size of the array
 */
template<typename T, std::size_t N>
class array {
public:
    // Member types
    using value_type             = T;
    using size_type              = std::size_t;
    using difference_type        = std::ptrdiff_t;
    using reference              = value_type&;
    using const_reference        = const value_type&;
    using pointer                = value_type*;
    using const_pointer          = const value_type*;
    using iterator               = pointer;
    using const_iterator         = const_pointer;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    // No explicit constructor or destructor needed
    
    // Element access
    [[nodiscard]] constexpr reference at(size_type pos) {
        if (pos >= N) {
            throw std::out_of_range("custom::array::at: pos (which is " + 
                                     std::to_string(pos) + ") >= N (which is " + 
                                     std::to_string(N) + ")");
        }
        return elements_[pos];
    }

    [[nodiscard]] constexpr const_reference at(size_type pos) const {
        if (pos >= N) {
            throw std::out_of_range("custom::array::at: pos (which is " + 
                                     std::to_string(pos) + ") >= N (which is " + 
                                     std::to_string(N) + ")");
        }
        return elements_[pos];
    }

    [[nodiscard]] constexpr reference operator[](size_type pos) noexcept {
        return elements_[pos];
    }

    [[nodiscard]] constexpr const_reference operator[](size_type pos) const noexcept {
        return elements_[pos];
    }

    [[nodiscard]] constexpr reference front() noexcept {
        return elements_[0];
    }

    [[nodiscard]] constexpr const_reference front() const noexcept {
        return elements_[0];
    }

    [[nodiscard]] constexpr reference back() noexcept {
        return elements_[N - 1];
    }

    [[nodiscard]] constexpr const_reference back() const noexcept {
        return elements_[N - 1];
    }

    [[nodiscard]] constexpr pointer data() noexcept {
        return elements_;
    }

    [[nodiscard]] constexpr const_pointer data() const noexcept {
        return elements_;
    }

    // Iterators
    [[nodiscard]] constexpr iterator begin() noexcept {
        return iterator(data());
    }

    [[nodiscard]] constexpr const_iterator begin() const noexcept {
        return const_iterator(data());
    }

    [[nodiscard]] constexpr const_iterator cbegin() const noexcept {
        return const_iterator(data());
    }

    [[nodiscard]] constexpr iterator end() noexcept {
        return iterator(data() + N);
    }

    [[nodiscard]] constexpr const_iterator end() const noexcept {
        return const_iterator(data() + N);
    }

    [[nodiscard]] constexpr const_iterator cend() const noexcept {
        return const_iterator(data() + N);
    }

    [[nodiscard]] constexpr reverse_iterator rbegin() noexcept {
        return reverse_iterator(end());
    }

    [[nodiscard]] constexpr const_reverse_iterator rbegin() const noexcept {
        return const_reverse_iterator(end());
    }

    [[nodiscard]] constexpr const_reverse_iterator crbegin() const noexcept {
        return const_reverse_iterator(cend());
    }

    [[nodiscard]] constexpr reverse_iterator rend() noexcept {
        return reverse_iterator(begin());
    }

    [[nodiscard]] constexpr const_reverse_iterator rend() const noexcept {
        return const_reverse_iterator(begin());
    }

    [[nodiscard]] constexpr const_reverse_iterator crend() const noexcept {
        return const_reverse_iterator(cbegin());
    }

    // Capacity
    [[nodiscard]] constexpr bool empty() const noexcept {
        return N == 0;
    }

    [[nodiscard]] constexpr size_type size() const noexcept {
        return N;
    }

    [[nodiscard]] constexpr size_type max_size() const noexcept {
        return N;
    }

    // Operations
    constexpr void fill(const T& value) {
        std::fill_n(elements_, N, value);
    }

    constexpr void swap(array& other) noexcept(std::is_nothrow_swappable_v<T>) {
        std::swap_ranges(begin(), end(), other.begin());
    }

    // Underlying array
    T elements_[N];
};

// Non-member functions

// Comparison operators
template<typename T, std::size_t N>
[[nodiscard]] constexpr bool operator==(const array<T, N>& lhs, const array<T, N>& rhs) {
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template<typename T, std::size_t N>
[[nodiscard]] constexpr bool operator!=(const array<T, N>& lhs, const array<T, N>& rhs) {
    return !(lhs == rhs);
}

template<typename T, std::size_t N>
[[nodiscard]] constexpr bool operator<(const array<T, N>& lhs, const array<T, N>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template<typename T, std::size_t N>
[[nodiscard]] constexpr bool operator<=(const array<T, N>& lhs, const array<T, N>& rhs) {
    return !(rhs < lhs);
}

template<typename T, std::size_t N>
[[nodiscard]] constexpr bool operator>(const array<T, N>& lhs, const array<T, N>& rhs) {
    return rhs < lhs;
}

template<typename T, std::size_t N>
[[nodiscard]] constexpr bool operator>=(const array<T, N>& lhs, const array<T, N>& rhs) {
    return !(lhs < rhs);
}

// Global swap for array
template<typename T, std::size_t N>
constexpr void swap(array<T, N>& lhs, array<T, N>& rhs) noexcept(noexcept(lhs.swap(rhs))) {
    lhs.swap(rhs);
}

// Specialized algorithms for array of size 0
template<class T>
class array<T, 0> {
public:
    // Member types
    using value_type             = T;
    using size_type              = std::size_t;
    using difference_type        = std::ptrdiff_t;
    using reference              = value_type&;
    using const_reference        = const value_type&;
    using pointer                = value_type*;
    using const_pointer          = const value_type*;
    using iterator               = pointer;
    using const_iterator         = const_pointer;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    // Element access
    [[nodiscard]] constexpr reference at(size_type pos) {
        throw std::out_of_range("custom::array<T, 0>::at: pos (which is " + 
                               std::to_string(pos) + ") >= 0");
    }

    [[nodiscard]] constexpr const_reference at(size_type pos) const {
        throw std::out_of_range("custom::array<T, 0>::at: pos (which is " + 
                               std::to_string(pos) + ") >= 0");
    }

    // These are undefined behavior in std::array<T, 0>, but we're being explicit
    [[nodiscard]] constexpr reference operator[](size_type) noexcept {
        // This should technically be undefined behavior
        static T dummy{};
        return dummy;
    }

    [[nodiscard]] constexpr const_reference operator[](size_type) const noexcept {
        // This should technically be undefined behavior
        static T dummy{};
        return dummy;
    }

    [[nodiscard]] constexpr reference front() noexcept {
        // Undefined behavior
        static T dummy{};
        return dummy;
    }

    [[nodiscard]] constexpr const_reference front() const noexcept {
        // Undefined behavior
        static T dummy{};
        return dummy;
    }

    [[nodiscard]] constexpr reference back() noexcept {
        // Undefined behavior
        static T dummy{};
        return dummy;
    }

    [[nodiscard]] constexpr const_reference back() const noexcept {
        // Undefined behavior
        static T dummy{};
        return dummy;
    }

    [[nodiscard]] constexpr pointer data() noexcept {
        return nullptr;
    }

    [[nodiscard]] constexpr const_pointer data() const noexcept {
        return nullptr;
    }

    // Iterators
    [[nodiscard]] constexpr iterator begin() noexcept {
        return iterator(data());
    }

    [[nodiscard]] constexpr const_iterator begin() const noexcept {
        return const_iterator(data());
    }

    [[nodiscard]] constexpr const_iterator cbegin() const noexcept {
        return const_iterator(data());
    }

    [[nodiscard]] constexpr iterator end() noexcept {
        return iterator(data());
    }

    [[nodiscard]] constexpr const_iterator end() const noexcept {
        return const_iterator(data());
    }

    [[nodiscard]] constexpr const_iterator cend() const noexcept {
        return const_iterator(data());
    }

    [[nodiscard]] constexpr reverse_iterator rbegin() noexcept {
        return reverse_iterator(end());
    }

    [[nodiscard]] constexpr const_reverse_iterator rbegin() const noexcept {
        return const_reverse_iterator(end());
    }

    [[nodiscard]] constexpr const_reverse_iterator crbegin() const noexcept {
        return const_reverse_iterator(cend());
    }

    [[nodiscard]] constexpr reverse_iterator rend() noexcept {
        return reverse_iterator(begin());
    }

    [[nodiscard]] constexpr const_reverse_iterator rend() const noexcept {
        return const_reverse_iterator(begin());
    }

    [[nodiscard]] constexpr const_reverse_iterator crend() const noexcept {
        return const_reverse_iterator(cbegin());
    }

    // Capacity
    [[nodiscard]] constexpr bool empty() const noexcept {
        return true;
    }

    [[nodiscard]] constexpr size_type size() const noexcept {
        return 0;
    }

    [[nodiscard]] constexpr size_type max_size() const noexcept {
        return 0;
    }

    // Operations
    constexpr void fill(const T&) {
        // No-op for empty array
    }

    constexpr void swap(array&) noexcept {
        // No-op for empty array
    }
};

// C++17 deduction guides
template<typename T, typename... U>
array(T, U...) -> array<T, 1 + sizeof...(U)>;

// Helper variable template for getting an array's size
template<typename T, std::size_t N>
constexpr std::size_t size(const array<T, N>&) noexcept {
    return N;
}

// Helper for tuple-like access
template<std::size_t I, typename T, std::size_t N>
constexpr T& get(array<T, N>& a) noexcept {
    static_assert(I < N, "Index out of bounds in custom::get");
    return a.elements_[I];
}

template<std::size_t I, typename T, std::size_t N>
constexpr const T& get(const array<T, N>& a) noexcept {
    static_assert(I < N, "Index out of bounds in custom::get");
    return a.elements_[I];
}

template<std::size_t I, typename T, std::size_t N>
constexpr T&& get(array<T, N>&& a) noexcept {
    static_assert(I < N, "Index out of bounds in custom::get");
    return std::move(a.elements_[I]);
}

template<std::size_t I, typename T, std::size_t N>
constexpr const T&& get(const array<T, N>&& a) noexcept {
    static_assert(I < N, "Index out of bounds in custom::get");
    return std::move(a.elements_[I]);
}

} // namespace custom

// Specialize std::tuple_size and std::tuple_element for custom::array
namespace std {
    template<typename T, size_t N>
    struct tuple_size<custom::array<T, N>> 
      : integral_constant<size_t, N> {};

    template<size_t I, typename T, size_t N>
    struct tuple_element<I, custom::array<T, N>> {
        static_assert(I < N, "Index out of bounds in std::tuple_element");
        using type = T;
    };
}