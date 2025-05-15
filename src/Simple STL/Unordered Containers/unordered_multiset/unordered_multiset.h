#ifndef UNORDERED_MULTISET_H
#define UNORDERED_MULTISET_H

#include <forward_list>
#include <functional>
#include <initializer_list>
#include <stdexcept>

namespace custom {

template <typename T, typename Hash = std::hash<T>, typename Equal = std::equal_to<T>>
class unordered_multiset {
private:
    using Bucket = std::forward_list<T>;
    Bucket* buckets;
    size_t bucket_count_;
    size_t size_;
    float max_load_factor_;
    Hash hash_fn;
    Equal equal_fn;

    size_t get_bucket_index(const T& value) const {
        return hash_fn(value) % bucket_count_;
    }

    void rehash() {
        size_t new_bucket_count = bucket_count_ * 2;
        Bucket* new_buckets = new Bucket[new_bucket_count];

        // Rehash all elements
        for (size_t i = 0; i < bucket_count_; ++i) {
            for (const auto& value : buckets[i]) {
                size_t new_index = hash_fn(value) % new_bucket_count;
                new_buckets[new_index].push_front(value);
            }
        }

        delete[] buckets;
        buckets = new_buckets;
        bucket_count_ = new_bucket_count;
    }

public:
    class iterator {
    private:
        Bucket* buckets;
        size_t bucket_count;
        size_t current_bucket;
        typename Bucket::iterator current_element;

        void find_next_element() {
            while (current_bucket < bucket_count) {
                if (current_element != buckets[current_bucket].end()) {
                    return;
                }
                ++current_bucket;
                if (current_bucket < bucket_count) {
                    current_element = buckets[current_bucket].begin();
                }
            }
        }

    public:
        iterator(Bucket* b, size_t bc, size_t cb, typename Bucket::iterator ce)
            : buckets(b), bucket_count(bc), current_bucket(cb), current_element(ce) {
            find_next_element();
        }

        T& operator*() {
            return *current_element;
        }

        iterator& operator++() {
            ++current_element;
            find_next_element();
            return *this;
        }

        iterator operator++(int) {
            iterator temp = *this;
            ++(*this);
            return temp;
        }

        bool operator==(const iterator& other) const {
            return current_bucket == other.current_bucket && 
                   current_element == other.current_element;
        }

        bool operator!=(const iterator& other) const {
            return !(*this == other);
        }
    };

    // Constructors
    unordered_multiset(size_t bucket_count = 16, const Hash& hash = Hash(), const Equal& equal = Equal())
        : bucket_count_(bucket_count), size_(0), max_load_factor_(1.0f), hash_fn(hash), equal_fn(equal) {
        buckets = new Bucket[bucket_count_];
    }

    unordered_multiset(std::initializer_list<T> init, size_t bucket_count = 16,
                      const Hash& hash = Hash(), const Equal& equal = Equal())
        : bucket_count_(bucket_count), size_(0), max_load_factor_(1.0f), hash_fn(hash), equal_fn(equal) {
        buckets = new Bucket[bucket_count_];
        for (const auto& value : init) {
            insert(value);
        }
    }

    // Copy constructor
    unordered_multiset(const unordered_multiset& other)
        : bucket_count_(other.bucket_count_), size_(0), max_load_factor_(other.max_load_factor_),
          hash_fn(other.hash_fn), equal_fn(other.equal_fn) {
        buckets = new Bucket[bucket_count_];
        for (const auto& value : other) {
            insert(value);
        }
    }

    // Move constructor
    unordered_multiset(unordered_multiset&& other) noexcept
        : buckets(other.buckets), bucket_count_(other.bucket_count_), size_(other.size_),
          max_load_factor_(other.max_load_factor_), hash_fn(std::move(other.hash_fn)),
          equal_fn(std::move(other.equal_fn)) {
        other.buckets = nullptr;
        other.bucket_count_ = 0;
        other.size_ = 0;
    }

    // Destructor
    ~unordered_multiset() {
        delete[] buckets;
    }

    // Assignment operators
    unordered_multiset& operator=(const unordered_multiset& other) {
        if (this != &other) {
            delete[] buckets;
            bucket_count_ = other.bucket_count_;
            size_ = 0;
            max_load_factor_ = other.max_load_factor_;
            hash_fn = other.hash_fn;
            equal_fn = other.equal_fn;
            buckets = new Bucket[bucket_count_];
            for (const auto& value : other) {
                insert(value);
            }
        }
        return *this;
    }

    unordered_multiset& operator=(unordered_multiset&& other) noexcept {
        if (this != &other) {
            delete[] buckets;
            buckets = other.buckets;
            bucket_count_ = other.bucket_count_;
            size_ = other.size_;
            max_load_factor_ = other.max_load_factor_;
            hash_fn = std::move(other.hash_fn);
            equal_fn = std::move(other.equal_fn);
            other.buckets = nullptr;
            other.bucket_count_ = 0;
            other.size_ = 0;
        }
        return *this;
    }

    unordered_multiset& operator=(std::initializer_list<T> init) {
        clear();
        for (const auto& value : init) {
            insert(value);
        }
        return *this;
    }

    // Iterators
    iterator begin() {
        return iterator(buckets, bucket_count_, 0, buckets[0].begin());
    }

    iterator end() {
        return iterator(buckets, bucket_count_, bucket_count_, buckets[bucket_count_ - 1].end());
    }

    // Capacity
    bool empty() const {
        return size_ == 0;
    }

    size_t size() const {
        return size_;
    }

    size_t bucket_count() const {
        return bucket_count_;
    }

    float load_factor() const {
        return static_cast<float>(size_) / bucket_count_;
    }

    float max_load_factor() const {
        return max_load_factor_;
    }

    void max_load_factor(float ml) {
        max_load_factor_ = ml;
        if (load_factor() > max_load_factor_) {
            rehash();
        }
    }

    // Modifiers
    void clear() {
        for (size_t i = 0; i < bucket_count_; ++i) {
            buckets[i].clear();
        }
        size_ = 0;
    }

    iterator insert(const T& value) {
        if (load_factor() > max_load_factor_) {
            rehash();
        }

        size_t index = get_bucket_index(value);
        buckets[index].push_front(value);
        ++size_;
        return iterator(buckets, bucket_count_, index, buckets[index].begin());
    }

    size_t erase(const T& value) {
        size_t index = get_bucket_index(value);
        auto& bucket = buckets[index];
        
        if (bucket.empty()) {
            return 0;
        }

        size_t erased = 0;
        auto prev = bucket.before_begin();
        auto curr = bucket.begin();
        while (curr != bucket.end()) {
            if (equal_fn(*curr, value)) {
                curr = bucket.erase_after(prev);
                ++erased;
                --size_;
            } else {
                ++prev;
                ++curr;
            }
        }
        return erased;
    }

    // Lookup
    iterator find(const T& value) {
        size_t index = get_bucket_index(value);
        for (auto it = buckets[index].begin(); it != buckets[index].end(); ++it) {
            if (equal_fn(*it, value)) {
                return iterator(buckets, bucket_count_, index, it);
            }
        }
        return end();
    }

    size_t count(const T& value) const {
        size_t index = get_bucket_index(value);
        size_t count = 0;
        for (const auto& element : buckets[index]) {
            if (equal_fn(element, value)) {
                ++count;
            }
        }
        return count;
    }

    bool contains(const T& value) const {
        return count(value) > 0;
    }
};

} // namespace custom

#endif // UNORDERED_MULTISET_H 