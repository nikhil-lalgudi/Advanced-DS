#ifndef CUSTOM_MULTIMAP_H
#define CUSTOM_MULTIMAP_H

#include <memory>
#include <functional>
#include <initializer_list>
#include <stdexcept>
#include <utility>

namespace custom {

template<typename Key, typename T, typename Compare = std::less<Key>>
class multimap {
private:
    struct Node {
        std::pair<const Key, T> data;
        Node* left;
        Node* right;
        Node* parent;
        bool is_black;  // true if black, false if red

        Node(const std::pair<const Key, T>& d, Node* p = nullptr) 
            : data(d), left(nullptr), right(nullptr), parent(p), is_black(false) {}
    };

    Node* root;
    size_t sz;
    Compare comp;

    // Helper functions
    void clear_helper(Node* node) {
        if (node) {
            clear_helper(node->left);
            clear_helper(node->right);
            delete node;
        }
    }

    Node* copy_helper(Node* node, Node* parent = nullptr) {
        if (!node) return nullptr;
        Node* new_node = new Node(node->data, parent);
        new_node->is_black = node->is_black;
        new_node->left = copy_helper(node->left, new_node);
        new_node->right = copy_helper(node->right, new_node);
        return new_node;
    }

    void rotate_left(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        if (y->left) y->left->parent = x;
        y->parent = x->parent;
        if (!x->parent) root = y;
        else if (x == x->parent->left) x->parent->left = y;
        else x->parent->right = y;
        y->left = x;
        x->parent = y;
    }

    void rotate_right(Node* x) {
        Node* y = x->left;
        x->left = y->right;
        if (y->right) y->right->parent = x;
        y->parent = x->parent;
        if (!x->parent) root = y;
        else if (x == x->parent->right) x->parent->right = y;
        else x->parent->left = y;
        y->right = x;
        x->parent = y;
    }

    void fix_insert(Node* k) {
        Node* u;
        while (k->parent && !k->parent->is_black) {
            if (k->parent == k->parent->parent->right) {
                u = k->parent->parent->left;
                if (u && !u->is_black) {
                    u->is_black = true;
                    k->parent->is_black = true;
                    k->parent->parent->is_black = false;
                    k = k->parent->parent;
                } else {
                    if (k == k->parent->left) {
                        k = k->parent;
                        rotate_right(k);
                    }
                    k->parent->is_black = true;
                    k->parent->parent->is_black = false;
                    rotate_left(k->parent->parent);
                }
            } else {
                u = k->parent->parent->right;
                if (u && !u->is_black) {
                    u->is_black = true;
                    k->parent->is_black = true;
                    k->parent->parent->is_black = false;
                    k = k->parent->parent;
                } else {
                    if (k == k->parent->right) {
                        k = k->parent;
                        rotate_left(k);
                    }
                    k->parent->is_black = true;
                    k->parent->parent->is_black = false;
                    rotate_right(k->parent->parent);
                }
            }
            if (k == root) break;
        }
        root->is_black = true;
    }

public:
    // Iterator class
    class iterator {
    private:
        Node* current;
        const multimap* container;

    public:
        iterator(Node* node = nullptr, const multimap* cont = nullptr) 
            : current(node), container(cont) {}

        std::pair<const Key, T>& operator*() { return current->data; }
        std::pair<const Key, T>* operator->() { return &current->data; }
        iterator& operator++() {
            if (!current) return *this;
            if (current->right) {
                current = current->right;
                while (current->left) current = current->left;
            } else {
                Node* parent = current->parent;
                while (parent && current == parent->right) {
                    current = parent;
                    parent = parent->parent;
                }
                current = parent;
            }
            return *this;
        }
        iterator operator++(int) {
            iterator temp = *this;
            ++(*this);
            return temp;
        }
        bool operator==(const iterator& other) const { return current == other.current; }
        bool operator!=(const iterator& other) const { return current != other.current; }
    };

    // Constructors
    multimap() : root(nullptr), sz(0) {}
    multimap(const Compare& comp) : root(nullptr), sz(0), comp(comp) {}
    multimap(const multimap& other) : root(nullptr), sz(0), comp(other.comp) {
        root = copy_helper(other.root);
        sz = other.sz;
    }
    multimap(multimap&& other) noexcept : root(other.root), sz(other.sz), comp(std::move(other.comp)) {
        other.root = nullptr;
        other.sz = 0;
    }
    multimap(std::initializer_list<std::pair<const Key, T>> init) : root(nullptr), sz(0) {
        for (const auto& pair : init) insert(pair);
    }

    // Destructor
    ~multimap() { clear(); }

    // Assignment operators
    multimap& operator=(const multimap& other) {
        if (this != &other) {
            clear();
            root = copy_helper(other.root);
            sz = other.sz;
            comp = other.comp;
        }
        return *this;
    }
    multimap& operator=(multimap&& other) noexcept {
        if (this != &other) {
            clear();
            root = other.root;
            sz = other.sz;
            comp = std::move(other.comp);
            other.root = nullptr;
            other.sz = 0;
        }
        return *this;
    }
    multimap& operator=(std::initializer_list<std::pair<const Key, T>> init) {
        clear();
        for (const auto& pair : init) insert(pair);
        return *this;
    }

    // Iterators
    iterator begin() const {
        if (!root) return end();
        Node* current = root;
        while (current->left) current = current->left;
        return iterator(current, this);
    }
    iterator end() const { return iterator(nullptr, this); }

    // Capacity
    bool empty() const { return sz == 0; }
    size_t size() const { return sz; }

    // Modifiers
    void clear() {
        clear_helper(root);
        root = nullptr;
        sz = 0;
    }

    iterator insert(const std::pair<const Key, T>& value) {
        Node* parent = nullptr;
        Node* current = root;
        bool is_left = false;

        while (current) {
            parent = current;
            if (comp(value.first, current->data.first)) {
                current = current->left;
                is_left = true;
            } else {
                current = current->right;
                is_left = false;
            }
        }

        Node* new_node = new Node(value, parent);
        if (!parent) {
            root = new_node;
        } else if (is_left) {
            parent->left = new_node;
        } else {
            parent->right = new_node;
        }

        fix_insert(new_node);
        ++sz;
        return iterator(new_node, this);
    }

    size_t erase(const Key& key) {
        // Implementation of erase would go here
        // This is a simplified version that just returns 0
        return 0;
    }

    // Lookup
    iterator find(const Key& key) const {
        Node* current = root;
        while (current) {
            if (comp(key, current->data.first)) {
                current = current->left;
            } else if (comp(current->data.first, key)) {
                current = current->right;
            } else {
                return iterator(current, this);
            }
        }
        return end();
    }

    size_t count(const Key& key) const {
        size_t count = 0;
        Node* current = root;
        while (current) {
            if (comp(key, current->data.first)) {
                current = current->left;
            } else if (comp(current->data.first, key)) {
                current = current->right;
            } else {
                ++count;
                current = current->right;  // Continue searching for duplicates
            }
        }
        return count;
    }

    bool contains(const Key& key) const {
        return find(key) != end();
    }
};

} // namespace custom

#endif // CUSTOM_MULTIMAP_H 