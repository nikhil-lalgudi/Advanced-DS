#pragma once
#include <algorithm>
#include <cstddef>
#include <iterator>
#include <memory>
#include <utility>

namespace custom {
template <typename T>
class forward_list {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& value) : data(value), next(nullptr) {}
        Node(T&& value) : data(std::move(value)), next(nullptr) {}
    };

public:
    using value_type      = T;
    using size_type       = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference       = value_type&;
    using const_reference = const value_type&;
    using pointer         = value_type*;
    using const_pointer   = const value_type*;

    class iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type        = T;
        using difference_type   = std::ptrdiff_t;
        using pointer           = T*;
        using reference         = T&;
        iterator() : current_(nullptr) {}
        explicit iterator(Node* node) : current_(node) {}
        reference operator*() const { return current_->data; }
        pointer operator->() const { return &(current_->data); }
        iterator& operator++() {
            current_ = current_->next;
            return *this;
        }
        iterator operator++(int) {
            iterator tmp = *this;
            current_ = current_->next;
            return tmp;
        }
        bool operator==(const iterator& other) const {
            return current_ == other.current_;
        }
        bool operator!=(const iterator& other) const {
            return !(*this == other);
        }
    private:
        Node* current_;
        friend class forward_list<T>;
    };

    class const_iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type        = T;
        using difference_type   = std::ptrdiff_t;
        using pointer           = const T*;
        using reference         = const T&;
        const_iterator() : current_(nullptr) {}
        explicit const_iterator(const Node* node) : current_(node) {}
        const_iterator(const iterator& it) : current_(it.current_) {}
        reference operator*() const { return current_->data; }
        pointer operator->() const { return &(current_->data); }
        const_iterator& operator++() {
            current_ = current_->next;
            return *this;
        }
        const_iterator operator++(int) {
            const_iterator tmp = *this;
            current_ = current_->next;
            return tmp;
        }
        bool operator==(const const_iterator& other) const {return current_ == other.current_;}
        bool operator!=(const const_iterator& other) const {return !(*this == other);}
    private:
        const Node* current_;
        friend class forward_list<T>;
    };
    // Default constructor
    forward_list() : head_(nullptr) {}
    // Constructor with count elements of value
    forward_list(size_type count, const T& value) : head_(nullptr) {
        for (size_type i = 0; i < count; ++i) {
            push_front(value);
        }
        reverse();
    }

    // Constructor with count default-inserted elements
    explicit forward_list(size_type count) : head_(nullptr) {
        for (size_type i = 0; i < count; ++i) {
            push_front(T());
        }
        reverse();
    }

    // Range constructor
    template <typename InputIt>
    forward_list(InputIt first, InputIt last) : head_(nullptr) {
        // Insert in reverse order and then reverse the list
        for (; first != last; ++first) {
            push_front(*first);
        }
        reverse();
    }

    forward_list(const forward_list& other) : head_(nullptr) {
        for (auto it = other.begin(); it != other.end(); ++it) {
            push_front(*it);
        }
        reverse();
    }

    forward_list(forward_list&& other) noexcept : head_(other.head_) {other.head_ = nullptr;}

    forward_list(std::initializer_list<T> init) : head_(nullptr) {
        for (auto it = init.begin(); it != init.end(); ++it) {
            push_front(*it);
        }
        reverse();
    }

    // Destructor
    ~forward_list() {
        clear();
    }

    // Assignment operators
    forward_list& operator=(const forward_list& other) {
        if (this != &other) {
            clear();
            for (auto it = other.begin(); it != other.end(); ++it) {
                push_front(*it);
            }
            reverse();
        }
        return *this;
    }

    forward_list& operator=(forward_list&& other) noexcept {
        if (this != &other) {
            clear();
            head_ = other.head_;
            other.head_ = nullptr;
        }
        return *this;
    }

    forward_list& operator=(std::initializer_list<T> ilist) {
        clear();
        for (auto it = ilist.begin(); it != ilist.end(); ++it) {
            push_front(*it);
        }
        reverse();
        return *this;
    }

    // Element access
    reference front() {
        return head_->data;
    }

    const_reference front() const {
        return head_->data;
    }

    // Iterators
    iterator before_begin() noexcept {
        static Node sentinel{T()};
        sentinel.next = head_;
        return iterator(&sentinel);
    }

    const_iterator before_begin() const noexcept {
        static Node sentinel{T()};
        sentinel.next = head_;
        return const_iterator(&sentinel);
    }

    iterator begin() noexcept {
        return iterator(head_);
    }

    const_iterator begin() const noexcept {
        return const_iterator(head_);
    }

    const_iterator cbegin() const noexcept {
        return const_iterator(head_);
    }

    iterator end() noexcept {
        return iterator(nullptr);
    }

    const_iterator end() const noexcept {
        return const_iterator(nullptr);
    }

    const_iterator cend() const noexcept {
        return const_iterator(nullptr);
    }

    // Capacity
    bool empty() const noexcept {
        return head_ == nullptr;
    }

    size_type size() const noexcept {
        size_type count = 0;
        for (auto it = begin(); it != end(); ++it) {
            ++count;
        }
        return count;
    }

    // Modifiers
    void clear() noexcept {
        while (head_ != nullptr) {
            Node* temp = head_;
            head_ = head_->next;
            delete temp;
        }
    }

    iterator insert_after(const_iterator pos, const T& value) {
        Node* new_node = new Node(value);
        Node* current = const_cast<Node*>(pos.current_);
        
        new_node->next = current->next;
        current->next = new_node;
        
        return iterator(new_node);
    }

    iterator insert_after(const_iterator pos, T&& value) {
        Node* new_node = new Node(std::move(value));
        Node* current = const_cast<Node*>(pos.current_);
        new_node->next = current->next;
        current->next = new_node;
        return iterator(new_node);
    }

    iterator erase_after(const_iterator pos) {
        Node* current = const_cast<Node*>(pos.current_);
        if (current->next == nullptr) {
            return end();
        }
        Node* to_erase = current->next;
        current->next = to_erase->next;
        iterator result(current->next);
        delete to_erase; return result;
    }

    iterator erase_after(const_iterator first, const_iterator last) {
        Node* current = const_cast<Node*>(first.current_);
        while (current->next != last.current_) {
            Node* to_erase = current->next;
            current->next = to_erase->next;
            delete to_erase;
        }
        
        return iterator(current->next);
    }

    void push_front(const T& value) {
        Node* new_node = new Node(value);
        new_node->next = head_;
        head_ = new_node;
    }

    void push_front(T&& value) {
        Node* new_node = new Node(std::move(value));
        new_node->next = head_;
        head_ = new_node;
    }

    void pop_front() {
        if (head_ != nullptr) {
            Node* temp = head_;
            head_ = head_->next;
            delete temp;
        }
    }

    void resize(size_type count, const value_type& value = value_type()) {
        // Count current size
        size_type current_size = size();

        if (count < current_size) {
            // Need to remove elements
            if (count == 0) {
                clear();
                return;
            }

            // Find node at position count-1
            Node* current = head_;
            for (size_type i = 0; i < count - 1; ++i) {
                current = current->next;
            }
            // Delete all nodes after current
            Node* to_delete = current->next;
            current->next = nullptr;
            while (to_delete != nullptr) {
                Node* next = to_delete->next;
                delete to_delete;
                to_delete = next;
            }
        } else if (count > current_size) {
            // Need to add elements
            // Find last node
            Node* current = head_;
            if (current == nullptr) {
                // List is empty
                for (size_type i = 0; i < count; ++i) {
                    push_front(value);
                }
                reverse();
                return;
            }
            while (current->next != nullptr) {
                current = current->next;
            }
            // Add new elements after last
            for (size_type i = current_size; i < count; ++i) {
                current->next = new Node(value);
                current = current->next;
            }
        }
    }

    void swap(forward_list& other) noexcept {
        std::swap(head_, other.head_);
    }

    // Operations
    void merge(forward_list& other) {
        if (this == &other || other.empty()) {
            return;
        }

        if (empty()) {
            head_ = other.head_;
            other.head_ = nullptr;
            return;
        }

        Node* current = nullptr;
        Node* first = head_;
        Node* second = other.head_;

        // Find which list has the smaller first element
        if (second->data < first->data) {
            current = second;
            second = second->next;
        } else {
            current = first;
            first = first->next;
        }

        // Save the new head
        head_ = current;

        // Merge the rest
        while (first != nullptr && second != nullptr) {
            if (second->data < first->data) {
                current->next = second;
                second = second->next;
            } else {
                current->next = first;
                first = first->next;
            }
            current = current->next;
        }

        // Attach remaining nodes
        current->next = (first != nullptr) ? first : second;

        other.head_ = nullptr;
    }

    void splice_after(const_iterator pos, forward_list& other) {
        if (other.empty()) {
            return;
        }
        
        Node* current = const_cast<Node*>(pos.current_);
        Node* last_in_other = other.head_;
        
        while (last_in_other->next != nullptr) {
            last_in_other = last_in_other->next;
        }
        
        last_in_other->next = current->next;
        current->next = other.head_;
        other.head_ = nullptr;
    }

    size_type remove(const T& value) {
        return remove_if([&value](const T& elem) { return elem == value; });
    }

    template <typename UnaryPredicate>
    size_type remove_if(UnaryPredicate p) {
        size_type count = 0;
        if (head_ == nullptr) {return count;}
        while (head_ != nullptr && p(head_->data)) {
            Node* to_delete = head_;
            head_ = head_->next;
            delete to_delete;
            ++count;
        }
        if (head_ == nullptr) {return count;}
        Node* current = head_;
        while (current->next != nullptr) {
            if (p(current->next->data)) {
                Node* to_delete = current->next;
                current->next = to_delete->next;
                delete to_delete;
                ++count;
            } else {
                current = current->next;
            }
        }
        return count;
    }

    void reverse() noexcept {
        if (head_ == nullptr || head_->next == nullptr) {return;}
        Node* prev = nullptr;
        Node* current = head_;
        Node* next = nullptr;
        while (current != nullptr) {
            next = current->next;
            current->next = prev;
            prev = current;
            current = next;
        }
        head_ = prev;
    }

    void sort() {
        if (head_ == nullptr || head_->next == nullptr) {return;}
        head_ = merge_sort(head_);
    }

private:
    Node* head_;
    // Merge sort implementation for linked list
    Node* merge_sort(Node* head) {
        // Base case: 0 or 1 node
        if (head == nullptr || head->next == nullptr) {return head;}
        Node* slow = head;
        Node* fast = head->next;
        while (fast != nullptr && fast->next != nullptr) {
            slow = slow->next;
            fast = fast->next->next;
        }
        Node* mid = slow->next;
        slow->next = nullptr;  // Break the list
        // Recursively sort the two halves
        Node* left = merge_sort(head);
        Node* right = merge_sort(mid);
        // Merge the sorted halves
        return merge_sorted_lists(left, right);
    }

    Node* merge_sorted_lists(Node* left, Node* right) {
        if (left == nullptr) {return right;}
        if (right == nullptr) {return left;}
        Node dummy{T()};  // Dummy node to simplify code
        Node* tail = &dummy;
        while (left != nullptr && right != nullptr) {
            if (left->data < right->data) {
                tail->next = left;
                left = left->next;
            } else {
                tail->next = right;
                right = right->next;
            }
            tail = tail->next;
        }
        tail->next = (left != nullptr) ? left : right;
        return dummy.next;
    }
};

// Comparison operators
template <typename T>
bool operator==(const forward_list<T>& lhs, const forward_list<T>& rhs) {
    auto it1 = lhs.begin(); auto it2 = rhs.begin();
    while (it1 != lhs.end() && it2 != rhs.end()) {
        if (!(*it1 == *it2)) {
            return false;
        }
        ++it1; ++it2;
    }
    return it1 == lhs.end() && it2 == rhs.end();
}
template <typename T>
bool operator!=(const forward_list<T>& lhs, const forward_list<T>& rhs) {
    return !(lhs == rhs);
}

template <typename T>
void swap(forward_list<T>& lhs, forward_list<T>& rhs) noexcept {
    lhs.swap(rhs);
}