#pragma once

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <memory>
#include <stdexcept> // For std::out_of_range
#include <utility>   // For std::move, std::swap

namespace custom {

/**
 * @brief A doubly-linked list container.
 *
 * @tparam T Type of the elements.
 */
template <typename T>
class list {
private:
    struct Node {
        T data;
        Node* prev;
        Node* next;

        Node(const T& value) : data(value), prev(nullptr), next(nullptr) {}
        Node(T&& value) : data(std::move(value)), prev(nullptr), next(nullptr) {}
    };

public:
    // Member types
    using value_type      = T;
    using size_type       = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference       = value_type&;
    using const_reference = const value_type&;
    using pointer         = value_type*;
    using const_pointer   = const value_type*;

    // Iterator
    class iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type        = T;
        using difference_type   = std::ptrdiff_t;
        using pointer           = T*;
        using reference         = T&;

        iterator() : node_(nullptr) {}
        explicit iterator(Node* node) : node_(node) {}

        reference operator*() const { return node_->data; }
        pointer operator->() const { return &(node_->data); }

        iterator& operator++() {
            node_ = node_->next;
            return *this;
        }

        iterator operator++(int) {
            iterator temp = *this;
            node_ = node_->next;
            return temp;
        }

        iterator& operator--() {
            node_ = node_->prev;
            return *this;
        }

        iterator operator--(int) {
            iterator temp = *this;
            node_ = node_->prev;
            return temp;
        }

        bool operator==(const iterator& other) const { return node_ == other.node_; }
        bool operator!=(const iterator& other) const { return node_ != other.node_; }

    private:
        Node* node_;
        friend class list<T>;
        friend class const_iterator;
    };

    // Const Iterator
    class const_iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type        = T;
        using difference_type   = std::ptrdiff_t;
        using pointer           = const T*;
        using reference         = const T&;

        const_iterator() : node_(nullptr) {}
        explicit const_iterator(const Node* node) : node_(node) {}
        const_iterator(const iterator& it) : node_(it.node_) {}

        reference operator*() const { return node_->data; }
        pointer operator->() const { return &(node_->data); }

        const_iterator& operator++() {
            node_ = node_->next;
            return *this;
        }

        const_iterator operator++(int) {
            const_iterator temp = *this;
            node_ = node_->next;
            return temp;
        }

        const_iterator& operator--() {
            node_ = node_->prev;
            return *this;
        }

        const_iterator operator--(int) {
            const_iterator temp = *this;
            node_ = node_->prev;
            return temp;
        }

        bool operator==(const const_iterator& other) const { return node_ == other.node_; }
        bool operator!=(const const_iterator& other) const { return node_ != other.node_; }

    private:
        const Node* node_;
        friend class list<T>;
    };
    
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;


private:
    Node* head_; // Points to a sentinel node before the first element
    Node* tail_; // Points to a sentinel node after the last element
    size_type size_;

    void init() {
        head_ = new Node(T()); // Sentinel node
        tail_ = new Node(T()); // Sentinel node
        head_->next = tail_;
        tail_->prev = head_;
        size_ = 0;
    }

public:
    // Constructors
    list() {
        init();
    }

    explicit list(size_type count, const T& value = T()) {
        init();
        for (size_type i = 0; i < count; ++i) {
            push_back(value);
        }
    }

    template <typename InputIt>
    list(InputIt first, InputIt last) {
        init();
        for (; first != last; ++first) {
            push_back(*first);
        }
    }

    list(const list& other) {
        init();
        for (const_reference val : other) {
            push_back(val);
        }
    }

    list(list&& other) noexcept : head_(other.head_), tail_(other.tail_), size_(other.size_) {
        other.head_ = nullptr; // Prevent double deletion
        other.tail_ = nullptr;
        other.size_ = 0;
    }
    
    list(std::initializer_list<T> ilist) {
        init();
        for (const_reference val : ilist) {
            push_back(val);
        }
    }

    // Destructor
    ~list() {
        clear();
        delete head_;
        delete tail_;
    }

    // Assignment operators
    list& operator=(const list& other) {
        if (this != &other) {
            clear(); // Clear current content
            // No need to delete/recreate sentinels, just head/tail links
            head_->next = tail_;
            tail_->prev = head_;
            size_ = 0;

            for (const_reference val : other) {
                push_back(val);
            }
        }
        return *this;
    }

    list& operator=(list&& other) noexcept {
        if (this != &other) {
            clear();      // Clear current content and delete old sentinels
            delete head_; 
            delete tail_;

            head_ = other.head_;
            tail_ = other.tail_;
            size_ = other.size_;

            other.head_ = nullptr; // Prevent double deletion
            other.tail_ = nullptr;
            other.size_ = 0;
        }
        return *this;
    }
    
    list& operator=(std::initializer_list<T> ilist) {
        clear();
        head_->next = tail_;
        tail_->prev = head_;
        size_ = 0;
        for (const_reference val : ilist) {
            push_back(val);
        }
        return *this;
    }

    // Element access
    reference front() {
        if (empty()) throw std::out_of_range("front() called on empty list");
        return head_->next->data;
    }

    const_reference front() const {
        if (empty()) throw std::out_of_range("front() called on empty list");
        return head_->next->data;
    }

    reference back() {
        if (empty()) throw std::out_of_range("back() called on empty list");
        return tail_->prev->data;
    }

    const_reference back() const {
        if (empty()) throw std::out_of_range("back() called on empty list");
        return tail_->prev->data;
    }

    // Iterators
    iterator begin() noexcept { return iterator(head_->next); }
    const_iterator begin() const noexcept { return const_iterator(head_->next); }
    const_iterator cbegin() const noexcept { return const_iterator(head_->next); }

    iterator end() noexcept { return iterator(tail_); }
    const_iterator end() const noexcept { return const_iterator(tail_); }
    const_iterator cend() const noexcept { return const_iterator(tail_); }
    
    reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
    const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(cend()); }

    reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
    const_reverse_iterator crend() const noexcept { return const_reverse_iterator(cbegin()); }


    // Capacity
    bool empty() const noexcept { return size_ == 0; }
    size_type size() const noexcept { return size_; }
    // max_size() is complex with allocators, omitting for simplicity

    // Modifiers
    void clear() noexcept {
        Node* current = head_->next;
        while (current != tail_) {
            Node* next_node = current->next;
            delete current;
            current = next_node;
        }
        head_->next = tail_;
        tail_->prev = head_;
        size_ = 0;
    }

    iterator insert(const_iterator pos, const T& value) {
        Node* current_node = const_cast<Node*>(pos.node_);
        Node* new_node = new Node(value);
        
        new_node->prev = current_node->prev;
        new_node->next = current_node;
        current_node->prev->next = new_node;
        current_node->prev = new_node;
        
        size_++;
        return iterator(new_node);
    }

    iterator insert(const_iterator pos, T&& value) {
        Node* current_node = const_cast<Node*>(pos.node_);
        Node* new_node = new Node(std::move(value));

        new_node->prev = current_node->prev;
        new_node->next = current_node;
        current_node->prev->next = new_node;
        current_node->prev = new_node;

        size_++;
        return iterator(new_node);
    }
    
    iterator insert(const_iterator pos, size_type count, const T& value) {
        if (count == 0) return iterator(const_cast<Node*>(pos.node_));
        iterator first_inserted_iter = end(); // Placeholder
        bool first = true;
        for (size_type i = 0; i < count; ++i) {
            iterator current_iter = insert(pos, value);
            if (first) {
                first_inserted_iter = current_iter;
                first = false;
            }
        }
        return first_inserted_iter;
    }

    template <typename InputIt>
    iterator insert(const_iterator pos, InputIt first, InputIt last) {
        if (first == last) return iterator(const_cast<Node*>(pos.node_));
        iterator first_inserted_iter = end(); 
        bool is_first_elem = true;
        for (; first != last; ++first) {
             iterator current_iter = insert(pos, *first);
             if(is_first_elem) {
                 first_inserted_iter = current_iter;
                 is_first_elem = false;
             }
        }
        return first_inserted_iter;
    }

    iterator insert(const_iterator pos, std::initializer_list<T> ilist) {
        return insert(pos, ilist.begin(), ilist.end());
    }


    iterator erase(const_iterator pos) {
        Node* node_to_erase = const_cast<Node*>(pos.node_);
        if (node_to_erase == tail_) return end(); // Cannot erase end sentinel

        Node* next_node = node_to_erase->next;
        node_to_erase->prev->next = next_node;
        next_node->prev = node_to_erase->prev;
        
        delete node_to_erase;
        size_--;
        return iterator(next_node);
    }
    
    iterator erase(const_iterator first, const_iterator last) {
        iterator current = iterator(const_cast<Node*>(first.node_));
        while (current != last) {
            current = erase(current); // erase returns iterator to the next element
        }
        return iterator(const_cast<Node*>(last.node_));
    }

    void push_back(const T& value) {
        insert(cend(), value);
    }

    void push_back(T&& value) {
        insert(cend(), std::move(value));
    }

    void pop_back() {
        if (!empty()) {
            erase(--cend());
        }
    }

    void push_front(const T& value) {
        insert(cbegin(), value);
    }

    void push_front(T&& value) {
        insert(cbegin(), std::move(value));
    }

    void pop_front() {
        if (!empty()) {
            erase(cbegin());
        }
    }
    
    void resize(size_type count, const T& value = T()) {
        if (count < size_) {
            while (size_ > count) {
                pop_back();
            }
        } else {
            while (size_ < count) {
                push_back(value);
            }
        }
    }

    void swap(list& other) noexcept {
        std::swap(head_, other.head_);
        std::swap(tail_, other.tail_);
        std::swap(size_, other.size_);
    }

    // Operations (subset for simplicity, more can be added)
    // splice (all forms are quite complex)
    // remove
    // remove_if
    // unique
    // merge
    // sort
    // reverse
};

// Non-member functions
template <typename T>
bool operator==(const list<T>& lhs, const list<T>& rhs) {
    if (lhs.size() != rhs.size()) return false;
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename T>
bool operator!=(const list<T>& lhs, const list<T>& rhs) {
    return !(lhs == rhs);
}

// <, <=, >, >= are typically lexicographical_compare
template <typename T>
bool operator<(const list<T>& lhs, const list<T>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename T>
bool operator<=(const list<T>& lhs, const list<T>& rhs) {
    return !(rhs < lhs);
}

template <typename T>
bool operator>(const list<T>& lhs, const list<T>& rhs) {
    return rhs < lhs;
}

template <typename T>
bool operator>=(const list<T>& lhs, const list<T>& rhs) {
    return !(lhs < rhs);
}

template <typename T>
void swap(list<T>& lhs, list<T>& rhs) noexcept {
    lhs.swap(rhs);
}

} // namespace custom 