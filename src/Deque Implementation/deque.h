#pragma once

#include <algorithm>
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <utility>
#include <iterator>

namespace custom {

/**
 * @brief A simplified double-ended queue implementation
 * 
 * This is a basic implementation using a single resizable buffer with
 * extra space at both ends to allow efficient insertion/removal operations.
 * 
 * @tparam T Type of the elements
 */
template<typename T>
class deque {
private:
    // Simple circular buffer implementation
    T* buffer_ = nullptr;
    size_t capacity_ = 0;
    size_t size_ = 0;
    size_t front_index_ = 0;  // Index of the first element
    
    // Helper to convert logical index to physical buffer index
    size_t to_physical_index(size_t logical_index) const {
        return (front_index_ + logical_index) % capacity_;
    }
    
    // Grow the buffer when needed
    void grow() {
        size_t new_capacity = capacity_ == 0 ? 8 : capacity_ * 2;
        T* new_buffer = new T[new_capacity];
        
        // Copy elements to the new buffer
        for (size_t i = 0; i < size_; ++i) {
            new_buffer[i] = std::move(buffer_[to_physical_index(i)]);
        }
        
        delete[] buffer_;
        buffer_ = new_buffer;
        capacity_ = new_capacity;
        front_index_ = 0;  // Reset front index in the new buffer
    }
    
public:
    // Member types
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using size_type = size_t;
    
    // Basic iterator implementation
    class iterator {
    private:
        deque* container_;
        size_t index_;
        
    public:
        using value_type = T;
        using reference = T&;
        using pointer = T*;
        using difference_type = ptrdiff_t;
        using iterator_category = std::random_access_iterator_tag;
        
        iterator(deque* container, size_t index) : container_(container), index_(index) {}
        
        reference operator*() { return (*container_)[index_]; }
        iterator& operator++() { ++index_; return *this; }
        iterator operator++(int) { iterator tmp = *this; ++index_; return tmp; }
        iterator& operator--() { --index_; return *this; }
        iterator operator--(int) { iterator tmp = *this; --index_; return tmp; }
        
        bool operator==(const iterator& other) const { return index_ == other.index_; }
        bool operator!=(const iterator& other) const { return index_ != other.index_; }
    };
    
    // Constructors and destructor
    deque() = default;
    
    explicit deque(size_type count, const T& value = T()) {
        capacity_ = count == 0 ? 0 : count * 2;  // Extra space for growth
        if (capacity_ > 0) {
            buffer_ = new T[capacity_];
            for (size_t i = 0; i < count; ++i) {
                buffer_[i] = value;
            }
            size_ = count;
        }
    }
    
    deque(const deque& other) {
        capacity_ = other.size_ * 2;  // Extra space
        if (capacity_ > 0) {
            buffer_ = new T[capacity_];
            for (size_t i = 0; i < other.size_; ++i) {
                buffer_[i] = other[i];
            }
            size_ = other.size_;
        }
    }
    
    deque(deque&& other) noexcept {
        buffer_ = other.buffer_;
        capacity_ = other.capacity_;
        size_ = other.size_;
        front_index_ = other.front_index_;
        
        other.buffer_ = nullptr;
        other.capacity_ = 0;
        other.size_ = 0;
        other.front_index_ = 0;
    }
    
    ~deque() {
        delete[] buffer_;
    }
    
    // Element access
    reference at(size_type pos) {
        if (pos >= size_) {
            throw std::out_of_range("deque::at: pos out of range");
        }
        return buffer_[to_physical_index(pos)];
    }
    
    const_reference at(size_type pos) const {
        if (pos >= size_) {
            throw std::out_of_range("deque::at: pos out of range");
        }
        return buffer_[to_physical_index(pos)];
    }
    
    reference operator[](size_type pos) {
        return buffer_[to_physical_index(pos)];
    }
    
    const_reference operator[](size_type pos) const {
        return buffer_[to_physical_index(pos)];
    }
    
    reference front() { return buffer_[front_index_]; }
    const_reference front() const { return buffer_[front_index_]; }
    
    reference back() { return buffer_[to_physical_index(size_ - 1)]; }
    const_reference back() const { return buffer_[to_physical_index(size_ - 1)]; }
    
    // Iterators
    iterator begin() { return iterator(this, 0); }
    iterator end() { return iterator(this, size_); }
    
    // Capacity
    [[nodiscard]] bool empty() const { return size_ == 0; }
    size_type size() const { return size_; }
    
    // Modifiers
    void push_back(const T& value) {
        if (size_ == capacity_) {
            grow();
        }
        buffer_[to_physical_index(size_)] = value;
        ++size_;
    }
    
    void push_front(const T& value) {
        if (size_ == capacity_) {
            grow();
        }
        front_index_ = (front_index_ + capacity_ - 1) % capacity_;
        buffer_[front_index_] = value;
        ++size_;
    }
    
    void pop_back() {
        if (!empty()) {
            --size_;
        }
    }
    
    void pop_front() {
        if (!empty()) {
            front_index_ = (front_index_ + 1) % capacity_;
            --size_;
        }
    }
    
    void clear() {
        size_ = 0;
        front_index_ = 0;
    }
};

} // namespace custom 