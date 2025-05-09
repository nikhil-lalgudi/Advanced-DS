#pragma once

#include <random>
#include <memory>
#include <vector>
#include <optional>
#include <algorithm>
#include <functional>
#include <type_traits>


template <typename T, typename Compare = std::less<T>>
class RandomBinaryTree {
private:
    struct Node {
        T key;
        int priority;  // Random priority for treap-like behavior
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;
        int size;  // Size of subtree rooted at this node
        
        Node(const T& k) : key(k), priority(std::rand()), left(nullptr), right(nullptr), size(1) {}
        
        [[nodiscard]] int left_size() const noexcept {
            return left ? left->size : 0;
        }
        
        [[nodiscard]] int right_size() const noexcept {
            return right ? right->size : 0;
        }
        
        void update_size() noexcept {
            size = 1 + left_size() + right_size();
        }
    };

    std::unique_ptr<Node> root_;
    Compare comp_;
    std::mt19937 rng_;
    
    /**
     * @brief Rotates the tree right at the given node
     * @param node Node to rotate
     * @return New root of the subtree
     */
    [[nodiscard]] std::unique_ptr<Node> rotate_right(std::unique_ptr<Node> node) noexcept {
        if (!node || !node->left) {
            return node;
        }
        
        auto new_root = std::move(node->left);
        node->left = std::move(new_root->right);
        node->update_size();
        
        new_root->right = std::move(node);
        new_root->update_size();
        
        return new_root;
    }
    
    /**
     * @brief Rotates the tree left at the given node
     * @param node Node to rotate
     * @return New root of the subtree
     */
    [[nodiscard]] std::unique_ptr<Node> rotate_left(std::unique_ptr<Node> node) noexcept {
        if (!node || !node->right) {
            return node;
        }
        
        auto new_root = std::move(node->right);
        node->right = std::move(new_root->left);
        node->update_size();
        
        new_root->left = std::move(node);
        new_root->update_size();
        
        return new_root;
    }
    
    /**
     * @brief Internal insert method that ensures randomized behavior
     * @param node Current node in the recursion
     * @param key Key to insert
     * @return New root of the subtree
     */
    [[nodiscard]] std::unique_ptr<Node> insert_internal(std::unique_ptr<Node> node, const T& key) noexcept {
        if (!node) {
            return std::make_unique<Node>(key);
        }
        
        if (comp_(key, node->key)) {
            // Insert into left subtree
            node->left = insert_internal(std::move(node->left), key);
            
            // Check if we need to rotate to maintain randomization property
            if (node->left && node->left->priority > node->priority) {
                node = rotate_right(std::move(node));
            }
        } else if (comp_(node->key, key)) {
            // Insert into right subtree
            node->right = insert_internal(std::move(node->right), key);
            
            // Check if we need to rotate to maintain randomization property
            if (node->right && node->right->priority > node->priority) {
                node = rotate_left(std::move(node));
            }
        }
        // If key already exists (equal), don't insert again
        
        node->update_size();
        return node;
    }
    
    /**
     * @brief Internal remove method
     * @param node Current node in the recursion
     * @param key Key to remove
     * @return New root of the subtree
     */
    [[nodiscard]] std::unique_ptr<Node> remove_internal(std::unique_ptr<Node> node, const T& key) noexcept {
        if (!node) {
            return nullptr;
        }
        
        if (comp_(key, node->key)) {
            // Key is in the left subtree
            node->left = remove_internal(std::move(node->left), key);
        } else if (comp_(node->key, key)) {
            // Key is in the right subtree
            node->right = remove_internal(std::move(node->right), key);
        } else {
            // Found the key, remove this node
            if (!node->left) {
                return std::move(node->right);
            }
            
            if (!node->right) {
                return std::move(node->left);
            }
            
            // Node has two children - find the successor or predecessor
            // and use randomization to decide which one to use
            std::uniform_int_distribution<> dist(0, 1);
            if (dist(rng_)) {
                // Use successor
                node = rotate_left(std::move(node));
                node->left = remove_internal(std::move(node->left), key);
            } else {
                // Use predecessor
                node = rotate_right(std::move(node));
                node->right = remove_internal(std::move(node->right), key);
            }
        }
        
        if (node) {
            node->update_size();
        }
        
        return node;
    }
    
    /**
     * @brief Internal find method
     * @param node Current node in the recursion
     * @param key Key to find
     * @return Pointer to node containing key or nullptr
     */
    [[nodiscard]] const Node* find_internal(const Node* node, const T& key) const noexcept {
        if (!node) {
            return nullptr;
        }
        
        if (comp_(key, node->key)) {
            return find_internal(node->left.get(), key);
        }
        
        if (comp_(node->key, key)) {
            return find_internal(node->right.get(), key);
        }
        
        return node; // Found the key
    }
    
    /**
     * @brief Internal method to find kth smallest element
     * @param node Current node in the recursion
     * @param k Position to find (0-based)
     * @return Pointer to kth node or nullptr
     */
    [[nodiscard]] const Node* select_internal(const Node* node, int k) const noexcept {
        if (!node) {
            return nullptr;
        }
        
        int left_size = node->left_size();
        
        if (k < left_size) {
            return select_internal(node->left.get(), k);
        } else if (k > left_size) {
            return select_internal(node->right.get(), k - left_size - 1);
        } else {
            return node; // k == left_size, this is the kth node
        }
    }

    /**
     * @brief Internal rank method
     * @param node Current node in the recursion
     * @param key Key to find rank for
     * @param accumulated_rank Rank accumulated so far
     * @return Rank of the key or -1 if not found
     */
    [[nodiscard]] int rank_internal(const Node* node, const T& key, int accumulated_rank) const noexcept {
        if (!node) {
            return -1;
        }
        
        if (comp_(key, node->key)) {
            return rank_internal(node->left.get(), key, accumulated_rank);
        }
        
        if (comp_(node->key, key)) {
            int right_rank = rank_internal(node->right.get(), key, accumulated_rank + node->left_size() + 1);
            return right_rank == -1 ? -1 : right_rank;
        }
        
        // Found the key
        return accumulated_rank + node->left_size();
    }
    
    /**
     * @brief Internal inorder traversal method
     * @param node Current node in the traversal
     * @param visitor Function to call for each key
     */
    void inorder_traversal_internal(const Node* node, const std::function<void(const T&)>& visitor) const noexcept {
        if (!node) {
            return;
        }
        
        inorder_traversal_internal(node->left.get(), visitor);
        visitor(node->key);
        inorder_traversal_internal(node->right.get(), visitor);
    }

public:
    /**
     * @brief Construct a new Random Binary Tree
     * @param seed Optional seed for the random number generator
     */
    explicit RandomBinaryTree(std::optional<unsigned int> seed = std::nullopt)
        : root_(nullptr), comp_(Compare()) {
        if (seed) {
            rng_.seed(*seed);
        } else {
            std::random_device rd;
            rng_.seed(rd());
        }
    }
    
    /**
     * @brief Insert a key into the tree
     * @param key Key to insert
     */
    void insert(const T& key) noexcept {
        root_ = insert_internal(std::move(root_), key);
    }
    
    /**
     * @brief Remove a key from the tree
     * @param key Key to remove
     * @return true if key was found and removed, false otherwise
     */
    [[nodiscard]] bool remove(const T& key) noexcept {
        auto old_root = root_ ? root_->size : 0;
        root_ = remove_internal(std::move(root_), key);
        return old_root != (root_ ? root_->size : 0);
    }
    
    /**
     * @brief Check if the tree contains a key
     * @param key Key to search for
     * @return true if key is in the tree, false otherwise
     */
    [[nodiscard]] bool contains(const T& key) const noexcept {
        return find_internal(root_.get(), key) != nullptr;
    }
    
    /**
     * @brief Get the size of the tree
     * @return Number of nodes in the tree
     */
    [[nodiscard]] int size() const noexcept {
        return root_ ? root_->size : 0;
    }
    
    /**
     * @brief Check if the tree is empty
     * @return true if tree is empty, false otherwise
     */
    [[nodiscard]] bool empty() const noexcept {
        return !root_;
    }
    
    /**
     * @brief Find the kth smallest element in the tree
     * @param k Position to find (0-based)
     * @return Optional containing the kth smallest element if it exists
     */
    [[nodiscard]] std::optional<T> select(int k) const noexcept {
        if (k < 0 || k >= size()) {
            return std::nullopt;
        }
        const Node* node = select_internal(root_.get(), k);
        return node ? std::optional<T>(node->key) : std::nullopt;
    }
    
    /**
     * @brief Find the rank of a key in the tree
     * @param key Key to find rank for
     * @return Rank of the key (number of elements smaller than key) or -1 if key not found
     */
    [[nodiscard]] int rank(const T& key) const noexcept {
        return rank_internal(root_.get(), key, 0);
    }
    
    /**
     * @brief Clear the tree
     */
    void clear() noexcept {
        root_.reset();
    }
    
    /**
     * @brief Perform an inorder traversal of the tree
     * @param visitor Function to call for each key
     */
    void inorder_traversal(const std::function<void(const T&)>& visitor) const noexcept {
        inorder_traversal_internal(root_.get(), visitor);
    }
    
    /**
     * @brief Get all keys in sorted order
     * @return Vector containing all keys in sorted order
     */
    [[nodiscard]] std::vector<T> to_sorted_vector() const noexcept {
        std::vector<T> result;
        result.reserve(size());
        inorder_traversal([&result](const T& key) {
            result.push_back(key);
        });
        return result;
    }
    
    /**
     * @brief Insert multiple keys into the tree
     * @param begin Iterator to the beginning of the range
     * @param end Iterator to the end of the range
     */
    template<typename InputIt>
    void insert(InputIt begin, InputIt end) noexcept {
        for (auto it = begin; it != end; ++it) {
            insert(*it);
        }
    }
    
    /**
     * @brief Insert multiple keys into the tree with randomization
     * @param begin Iterator to the beginning of the range
     * @param end Iterator to the end of the range
     * 
     * This method shuffles the elements before insertion to ensure randomization
     */
    template<typename InputIt>
    void insert_randomized(InputIt begin, InputIt end) noexcept {
        std::vector<typename std::iterator_traits<InputIt>::value_type> values(begin, end);
        std::shuffle(values.begin(), values.end(), rng_);
        for (const auto& value : values) {
            insert(value);
        }
    }
    
    /**
     * @brief Build a random binary tree from a range of values
     * @param begin Iterator to the beginning of the range
     * @param end Iterator to the end of the range
     * @return A new random binary tree containing the elements
     */
    template<typename InputIt>
    static RandomBinaryTree build_random_tree(InputIt begin, InputIt end) {
        RandomBinaryTree tree;
        tree.insert_randomized(begin, end);
        return tree;
    }
};
