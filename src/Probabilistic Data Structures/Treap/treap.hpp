#pragma once

#include <random>
#include <functional>
#include <memory>
#include <optional>
#include <stdexcept>
#include <utility>
#include <cstddef>

namespace ds {
    template <typename Key, typename Compare = std::less<Key>, 
              typename Random = std::mt19937_64>
    class Treap {
    public:
        using key_type = Key;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;

    private:
        struct Node {
            Key key;
            unsigned priority;
            std::unique_ptr<Node> left;
            std::unique_ptr<Node> right;
            size_type subtree_size;

            Node(const Key& k, unsigned p) 
                : key(k), priority(p), subtree_size(1) {}
        };

        std::unique_ptr<Node> root;
        Compare comp;
        Random rng;

        // Utility functions
        [[nodiscard]] static size_type get_size(const Node* node) noexcept {
            return node ? node->subtree_size : 0;
        }

        void update_size(Node* node) noexcept {
            if (node) {
                node->subtree_size = 1 + get_size(node->left.get()) + 
                                        get_size(node->right.get());
            }
        }

        [[nodiscard]] std::pair<std::unique_ptr<Node>, std::unique_ptr<Node>> 
        split(std::unique_ptr<Node> node, const Key& key) {
            if (!node) {
                return {nullptr, nullptr};
            }

            if (comp(key, node->key)) {
                auto [left, right] = split(std::move(node->left), key);
                node->left = std::move(right);
                update_size(node.get());
                return {std::move(left), std::move(node)};
            } else {
                auto [left, right] = split(std::move(node->right), key);
                node->right = std::move(left);
                update_size(node.get());
                return {std::move(node), std::move(right)};
            }
        }

        std::unique_ptr<Node> merge(std::unique_ptr<Node> left, 
                                    std::unique_ptr<Node> right) {
            if (!left) return right;
            if (!right) return left;

            if (left->priority > right->priority) {
                left->right = merge(std::move(left->right), std::move(right));
                update_size(left.get());
                return left;
            } else {
                right->left = merge(std::move(left), std::move(right->left));
                update_size(right.get());
                return right;
            }
        }

        [[nodiscard]] Node* find_impl(const Key& key) const {
            Node* current = root.get();
            while (current) {
                if (key == current->key) return current;
                current = comp(key, current->key) ? 
                          current->left.get() : current->right.get();
            }
            return nullptr;
        }

        [[nodiscard]] Node* kth_element_impl(size_type k) const {
            if (k >= size()) return nullptr;

            Node* current = root.get();
            while (current) {
                size_type left_size = get_size(current->left.get());
                if (k < left_size) {
                    current = current->left.get();
                } else if (k > left_size) {
                    k -= left_size + 1;
                    current = current->right.get();
                } else {
                    return current;
                }
            }
            return nullptr;
        }

    public:
        Treap() = default;
        Treap(const Compare& comp_arg) : comp(comp_arg), rng(std::random_device{}()) {}

        // Copy and move semantics
        Treap(const Treap&) = delete;
        Treap& operator=(const Treap&) = delete;
        
        Treap(Treap&& other) noexcept = default;
        Treap& operator=(Treap&& other) noexcept = default;

        [[nodiscard]] bool empty() const noexcept {
            return !root;
        }

        [[nodiscard]] size_type size() const noexcept {
            return get_size(root.get());
        }

        [[nodiscard]] bool contains(const Key& key) const {
            return find_impl(key) != nullptr;
        }

        [[nodiscard]] std::optional<Key> find_kth(size_type k) const {
            Node* node = kth_element_impl(k);
            return node ? std::optional<Key>(node->key) : std::nullopt;
        }

        void insert(const Key& key) {
            // If key already exists, do nothing
            if (contains(key)) return;

            unsigned priority = std::uniform_int_distribution<unsigned>{}(rng);
            auto new_node = std::make_unique<Node>(key, priority);

            if (!root) {
                root = std::move(new_node);
                return;
            }

            auto [left, right] = split(std::move(root), key);
            
            root = merge(std::move(left), 
                         merge(std::move(new_node), std::move(right)));
        }

        void erase(const Key& key) {
            if (!root) return;

            auto [left, right_subtree] = split(std::move(root), key);
            auto [equal, right] = split(std::move(right_subtree), 
                                        [&key](const auto& k) { return k > key; });

            root = merge(std::move(left), std::move(right));
        }

        [[nodiscard]] size_type order_of_key(const Key& key) const {
            size_type less_count = 0;
            Node* current = root.get();

            while (current) {
                if (comp(key, current->key)) {
                    current = current->left.get();
                } else if (key == current->key) {
                    less_count += get_size(current->left.get());
                    break;
                } else {
                    less_count += get_size(current->left.get()) + 1;
                    current = current->right.get();
                }
            }

            return less_count;
        }

        void clear() noexcept {
            root.reset();
        }
    };
} 