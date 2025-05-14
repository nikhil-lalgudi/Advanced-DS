#pragma once

#include "../../Sequential Containers/forward_list/forwardlist.h" // For buckets (chaining)
#include "../../Sequential Containers/vector/vector.h"     // For the bucket array
#include <functional> // For std::hash, std::equal_to
#include <utility>    // For std::move, std::pair, std::forward
#include <algorithm>  // For std::max
#include <cmath>      // For std::ceil

namespace custom {

/**
 * @brief An unordered set container (hash set) using chaining for collision resolution.
 *
 * @tparam Key Type of the elements.
 * @tparam Hash Hash function object type, defaults to std::hash<Key>.
 * @tparam KeyEqual Equality comparison function object type, defaults to std::equal_to<Key>.
 * @tparam Allocator Allocator type (simplified, mainly for vector of buckets, not per-element).
 */
template <typename Key,
          typename Hash = std::hash<Key>,
          typename KeyEqual = std::equal_to<Key>,
          typename Allocator = std::allocator<custom::forward_list<Key>>> // Allocator for the vector of buckets
class unordered_set {
public:
    // Member types
    using key_type        = Key;
    using value_type      = Key; // For set, key_type and value_type are the same
    using size_type       = std::size_t;
    using difference_type = std::ptrdiff_t;
    using hasher          = Hash;
    using key_equal       = KeyEqual;
    using allocator_type  = Allocator;
    using reference       = value_type&; // Iterators will point to value_type
    using const_reference = const value_type&;
    // Pointer types would typically use allocator_traits, but iterators are more complex here.

private:
    using Bucket = custom::forward_list<value_type>; // Each bucket is a forward_list
    using BucketVector = custom::vector<Bucket, Allocator>; // Vector of buckets

    BucketVector buckets_;
    size_type num_elements_;
    float max_load_factor_;
    Hash hash_fn_;
    KeyEqual key_eq_fn_;

    // Default initial bucket count and load factor
    static constexpr size_type DEFAULT_BUCKET_COUNT = 10;
    static constexpr float DEFAULT_MAX_LOAD_FACTOR = 1.0f;

public:
    // Iterators (Simplified: forward iterator through all elements)
    // A proper unordered_set iterator is more complex, needing to skip empty buckets
    // and iterate through the forward_list in each non-empty bucket.
    // This is a very simplified iterator for basic traversal.
    class iterator { // Minimal const_iterator like behavior
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type        = const Key; // Elements are const in a set iterator
        using difference_type   = std::ptrdiff_t;
        using pointer           = const Key*;
        using reference         = const Key&;
    
    private:
        typename BucketVector::const_iterator bucket_iter_;
        typename BucketVector::const_iterator bucket_end_;
        typename Bucket::const_iterator element_iter_;

        void advance_to_next_valid() {
            while (bucket_iter_ != bucket_end_) {
                if (element_iter_ != bucket_iter_->end()) {
                    return; // Found an element
                }
                // Move to next bucket
                ++bucket_iter_;
                if (bucket_iter_ != bucket_end_) {
                    element_iter_ = bucket_iter_->begin();
                }
            }
            // Reached end of all buckets
        }

    public:
        iterator(typename BucketVector::const_iterator bucket_start, 
                 typename BucketVector::const_iterator bucket_end) 
            : bucket_iter_(bucket_start), bucket_end_(bucket_end), element_iter_() {
            if (bucket_iter_ != bucket_end_) {
                element_iter_ = bucket_iter_->begin();
            }
            advance_to_next_valid();
        }
        
        // End iterator constructor
        iterator(typename BucketVector::const_iterator bucket_end_iter) 
            : bucket_iter_(bucket_end_iter), bucket_end_(bucket_end_iter), element_iter_() {}


        reference operator*() const { return *element_iter_; }
        pointer operator->() const { return &(*element_iter_); }

        iterator& operator++() {
            if (bucket_iter_ != bucket_end_) {
                ++element_iter_;
                advance_to_next_valid();
            }
            return *this;
        }

        iterator operator++(int) {
            iterator temp = *this;
            ++(*this);
            return temp;
        }

        bool operator==(const iterator& other) const {
            // For end iterators, only bucket_iter_ comparison is enough if both are end.
            if (bucket_iter_ == bucket_end_ && other.bucket_iter_ == other.bucket_end_) return true;
            return bucket_iter_ == other.bucket_iter_ && element_iter_ == other.element_iter_;
        }
        bool operator!=(const iterator& other) const { return !(*this == other); }
    };
    using const_iterator = iterator; // For set, iterator is effectively const_iterator

private:
    size_type get_bucket_index(const Key& key) const {
        if (buckets_.empty()) return 0; // Should not happen if initialized
        return hash_fn_(key) % buckets_.size();
    }

    void rehash_if_needed() {
        if (buckets_.empty() || (static_cast<float>(num_elements_) / buckets_.size()) > max_load_factor_) {
            rehash(std::max(DEFAULT_BUCKET_COUNT, static_cast<size_type>(std::ceil(num_elements_ / max_load_factor_)) * 2));
        }
    }

public:
    // Constructors
    unordered_set(size_type bucket_count = DEFAULT_BUCKET_COUNT,
                  const Hash& hash = Hash(),
                  const KeyEqual& equal = KeyEqual(),
                  const Allocator& alloc = Allocator())
        : buckets_(alloc), num_elements_(0), max_load_factor_(DEFAULT_MAX_LOAD_FACTOR),
          hash_fn_(hash), key_eq_fn_(equal) {
        buckets_.resize(std::max(bucket_count, static_cast<size_type>(1))); // Min 1 bucket
    }

    template <typename InputIt>
    unordered_set(InputIt first, InputIt last,
                  size_type bucket_count = DEFAULT_BUCKET_COUNT,
                  const Hash& hash = Hash(),
                  const KeyEqual& equal = KeyEqual(),
                  const Allocator& alloc = Allocator())
        : unordered_set(bucket_count, hash, equal, alloc) {
        insert(first, last);
    }
    
    unordered_set(const unordered_set& other)
        : buckets_(other.buckets_), // This does a deep copy of vector of lists
          num_elements_(other.num_elements_),
          max_load_factor_(other.max_load_factor_),
          hash_fn_(other.hash_fn_),
          key_eq_fn_(other.key_eq_fn_) {}

    unordered_set(unordered_set&& other) noexcept
        : buckets_(std::move(other.buckets_)),
          num_elements_(other.num_elements_),
          max_load_factor_(other.max_load_factor_),
          hash_fn_(std::move(other.hash_fn_)),
          key_eq_fn_(std::move(other.key_eq_fn_)) {
        other.num_elements_ = 0;
    }
    
    unordered_set(std::initializer_list<value_type> ilist,
                  size_type bucket_count = DEFAULT_BUCKET_COUNT,
                  const Hash& hash = Hash(),
                  const KeyEqual& equal = KeyEqual(),
                  const Allocator& alloc = Allocator())
        : unordered_set(bucket_count, hash, equal, alloc) {
        insert(ilist.begin(), ilist.end());
    }

    // Destructor - default is fine as members handle their own memory

    // Assignment operators
    unordered_set& operator=(const unordered_set& other) {
        if (this != &other) {
            buckets_ = other.buckets_; // Deep copy
            num_elements_ = other.num_elements_;
            max_load_factor_ = other.max_load_factor_;
            hash_fn_ = other.hash_fn_;
            key_eq_fn_ = other.key_eq_fn_;
        }
        return *this;
    }

    unordered_set& operator=(unordered_set&& other) noexcept {
        if (this != &other) {
            buckets_ = std::move(other.buckets_);
            num_elements_ = other.num_elements_;
            max_load_factor_ = other.max_load_factor_;
            hash_fn_ = std::move(other.hash_fn_);
            key_eq_fn_ = std::move(other.key_eq_fn_);
            other.num_elements_ = 0;
            other.buckets_.clear(); // Ensure moved-from state is clean
        }
        return *this;
    }
    
    unordered_set& operator=(std::initializer_list<value_type> ilist) {
        clear();
        // Could try to keep current bucket_count if reasonable, or reset
        // For simplicity, let's use the default bucket count or estimate from ilist.size()
        rehash(std::max(DEFAULT_BUCKET_COUNT, static_cast<size_type>(std::ceil(ilist.size() / max_load_factor_))));
        insert(ilist.begin(), ilist.end());
        return *this;
    }

    // Iterators
    iterator begin() const { return iterator(buckets_.cbegin(), buckets_.cend()); }
    const_iterator cbegin() const { return begin(); }
    iterator end() const { return iterator(buckets_.cend()); } // End iterator points past all buckets
    const_iterator cend() const { return end(); }

    // Capacity
    bool empty() const noexcept { return num_elements_ == 0; }
    size_type size() const noexcept { return num_elements_; }
    // max_size is complex, related to allocator & bucket vector limits

    // Modifiers
    void clear() noexcept {
        for (auto& bucket : buckets_) {
            bucket.clear();
        }
        num_elements_ = 0;
        // Buckets vector itself is not cleared, just its contents.
    }

    std::pair<iterator, bool> insert(const value_type& value) {
        rehash_if_needed();
        size_type bucket_idx = get_bucket_index(value);
        Bucket& bucket = buckets_[bucket_idx];
        for (auto it = bucket.begin(); it != bucket.end(); ++it) {
            if (key_eq_fn_(*it, value)) {
                // Element already exists, return iterator to it and false
                // This simplified iterator doesn't easily point back. Re-finding for now.
                return {find(value), false};
            }
        }
        bucket.push_front(value); // Or push_back, doesn't matter for set semantics in bucket
        num_elements_++;
        // Return iterator to the new element and true
        // This is tricky with the current iterator design. Returning find() is safest.
        return {find(value), true};
    }

    std::pair<iterator, bool> insert(value_type&& value) {
        rehash_if_needed();
        size_type bucket_idx = get_bucket_index(value);
        Bucket& bucket = buckets_[bucket_idx];
        for (auto it = bucket.begin(); it != bucket.end(); ++it) {
            if (key_eq_fn_(*it, value)) {
                return {find(value), false};
            }
        }
        bucket.push_front(std::move(value));
        num_elements_++;
        return {find(value), true};
    }
    
    // iterator insert(const_iterator hint, const value_type& value) - hint is complex, omitting
    // iterator insert(const_iterator hint, value_type&& value) - hint is complex, omitting

    template <typename InputIt>
    void insert(InputIt first, InputIt last) {
        for (; first != last; ++first) {
            insert(*first);
        }
    }
    
    void insert(std::initializer_list<value_type> ilist) {
        insert(ilist.begin(), ilist.end());
    }

    // emplace and emplace_hint are more complex, similar to insert

    size_type erase(const key_type& key) {
        size_type bucket_idx = get_bucket_index(key);
        if (bucket_idx >= buckets_.size()) return 0; // Should not happen
        
        Bucket& bucket = buckets_[bucket_idx];
        auto prev_it = bucket.before_begin();
        for (auto it = bucket.begin(); it != bucket.end(); ++it, ++prev_it) {
            if (key_eq_fn_(*it, key)) {
                bucket.erase_after(prev_it);
                num_elements_--;
                return 1;
            }
        }
        return 0;
    }
    
    // iterator erase(const_iterator pos) - very complex to implement correctly with this iterator
    // iterator erase(const_iterator first, const_iterator last) - also complex

    void swap(unordered_set& other) noexcept {
        using std::swap;
        swap(buckets_, other.buckets_);
        swap(num_elements_, other.num_elements_);
        swap(max_load_factor_, other.max_load_factor_);
        swap(hash_fn_, other.hash_fn_);
        swap(key_eq_fn_, other.key_eq_fn_);
    }

    // Lookup
    size_type count(const Key& key) const {
        size_type bucket_idx = get_bucket_index(key);
        if (bucket_idx >= buckets_.size()) return 0;
        const Bucket& bucket = buckets_[bucket_idx];
        for (const auto& elem : bucket) {
            if (key_eq_fn_(elem, key)) {
                return 1;
            }
        }
        return 0;
    }

    iterator find(const Key& key) const {
        size_type bucket_idx = get_bucket_index(key);
        if (bucket_idx >= buckets_.size()) return end();
        
        // Need to iterate to the specific element if found
        auto current_bucket_iter = buckets_.cbegin() + bucket_idx;
        auto it_in_bucket = current_bucket_iter->begin();
        while(it_in_bucket != current_bucket_iter->end()){
            if(key_eq_fn_(*it_in_bucket, key)){
                // Construct an iterator that points here
                iterator found_iter(buckets_.cbegin(), buckets_.cend());
                found_iter.bucket_iter_ = current_bucket_iter;
                found_iter.element_iter_ = it_in_bucket;
                return found_iter;
            }
            ++it_in_bucket;
        }
        return end();
    }
    
    bool contains(const Key& key) const {
        return find(key) != end();
    }

    // Bucket interface
    size_type bucket_count() const noexcept { return buckets_.size(); }
    // max_bucket_count() depends on vector's max_size
    // size_type bucket_size(size_type n) const; // Would need to iterate list
    // local_iterator bucket_begin(size_type n); // More complex iterators
    // const_local_iterator bucket_cbegin(size_type n) const;

    // Hash policy
    float load_factor() const noexcept {
        return buckets_.empty() ? 0.0f : static_cast<float>(num_elements_) / buckets_.size();
    }
    float max_load_factor() const noexcept { return max_load_factor_; }
    void max_load_factor(float ml) { 
        max_load_factor_ = ml; 
        rehash_if_needed(); // Or rehash(bucket_count()) to maintain current buckets if load factor allows
    }

    void rehash(size_type count) {
        if (count == buckets_.size()) return;
        count = std::max(count, static_cast<size_type>(std::ceil(num_elements_ / max_load_factor_)));
        if (count == buckets_.size()) return; // If adjusted count is same, no-op

        BucketVector old_buckets = std::move(buckets_); // Steal old buckets
        
        buckets_.assign(count, Bucket()); // Create new empty buckets of desired allocator later
                                        // For now, default allocator for BucketVector.
        num_elements_ = 0; // Will be recounted

        for (const auto& bucket : old_buckets) {
            for (const auto& key : bucket) {
                insert(key); // Re-insert all elements into new table
            }
        }
    }
    void reserve(size_type count) { // Ensure enough buckets for at least 'count' elements
        rehash(static_cast<size_type>(std::ceil(count / max_load_factor_)));
    }

    // Observers
    hasher hash_function() const { return hash_fn_; }
    key_equal key_eq() const { return key_eq_fn_; }
    // allocator_type get_allocator() const; // If using stateful allocator for vector

};

// No standard comparison operators for unordered_set itself beyond equality (element-wise, order-independent)
// For simplicity, operator== is omitted as it's non-trivial.

// Non-member swap
template <typename Key, typename Hash, typename KeyEqual, typename Alloc>
void swap(unordered_set<Key, Hash, KeyEqual, Alloc>& lhs, 
          unordered_set<Key, Hash, KeyEqual, Alloc>& rhs) noexcept(noexcept(lhs.swap(rhs))) {
    lhs.swap(rhs);
}

} // namespace custom 