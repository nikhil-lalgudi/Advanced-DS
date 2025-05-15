#ifndef CUSTOM_UNORDERED_MAP_H
#define CUSTOM_UNORDERED_MAP_H

#include <memory>
#include <functional>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include "vector.h"
#include "forwardlist.h"

namespace custom {

template<typename Key, typename T, typename Hash = std::hash<Key>, typename KeyEqual = std::equal_to<Key>>
class unordered_map {
private:
    using value_type = std::pair<const Key, T>;
    using bucket_type = forward_list<value_type>;
    using bucket_array = vector<bucket_type>;

    bucket_array buckets;
    size_t sz;
    float max_load_factor;
    Hash hash;
    KeyEqual key_equal;

    size_t get_bucket_index(const Key& key) const {
        return hash(key) % buckets.size();
    }

    void rehash(size_t new_size) {
        bucket_array new_buckets(new_size);
        for (auto& bucket : buckets) {
            for (auto it = bucket.begin(); it != bucket.end(); ++it) {
                size_t new_index = hash(it->first) % new_size;
                new_buckets[new_index].push_front(*it);
            }
        }
        buckets = std::move(new_buckets);
    }

    void check_rehash() {
        if (load_factor() > max_load_factor) {
            rehash(buckets.size() * 2);
        }
    }

public:
    // Iterator class
    class iterator {
    private:
        bucket_array* buckets_ptr;
        size_t bucket_index;
        typename bucket_type::iterator list_it;

    public:
        iterator(bucket_array* buckets = nullptr, size_t bucket = 0, 
                typename bucket_type::iterator it = typename bucket_type::iterator())
            : buckets_ptr(buckets), bucket_index(bucket), list_it(it) {}

        value_type& operator*() { return *list_it; }
        value_type* operator->() { return &(*list_it); }
        iterator& operator++() {
            ++list_it;
            if (list_it == (*buckets_ptr)[bucket_index].end()) {
                ++bucket_index;
                while (bucket_index < buckets_ptr->size() && (*buckets_ptr)[bucket_index].empty()) {
                    ++bucket_index;
                }
                if (bucket_index < buckets_ptr->size()) {
                    list_it = (*buckets_ptr)[bucket_index].begin();
                }
            }
            return *this;
        }
        iterator operator++(int) {
            iterator temp = *this;
            ++(*this);
            return temp;
        }
        bool operator==(const iterator& other) const {
            return buckets_ptr == other.buckets_ptr && 
                   bucket_index == other.bucket_index && 
                   list_it == other.list_it;
        }
        bool operator!=(const iterator& other) const {
            return !(*this == other);
        }
    };

    // Constructors
    unordered_map() : buckets(10), sz(0), max_load_factor(1.0f) {}
    unordered_map(const Hash& h, const KeyEqual& ke) 
        : buckets(10), sz(0), max_load_factor(1.0f), hash(h), key_equal(ke) {}
    unordered_map(const unordered_map& other) 
        : buckets(other.buckets), sz(other.sz), max_load_factor(other.max_load_factor),
          hash(other.hash), key_equal(other.key_equal) {}
    unordered_map(unordered_map&& other) noexcept 
        : buckets(std::move(other.buckets)), sz(other.sz), max_load_factor(other.max_load_factor),
          hash(std::move(other.hash)), key_equal(std::move(other.key_equal)) {
        other.sz = 0;
    }
    unordered_map(std::initializer_list<value_type> init) : buckets(10), sz(0), max_load_factor(1.0f) {
        for (const auto& pair : init) insert(pair);
    }

    // Destructor
    ~unordered_map() = default;

    // Assignment operators
    unordered_map& operator=(const unordered_map& other) {
        if (this != &other) {
            buckets = other.buckets;
            sz = other.sz;
            max_load_factor = other.max_load_factor;
            hash = other.hash;
            key_equal = other.key_equal;
        }
        return *this;
    }
    unordered_map& operator=(unordered_map&& other) noexcept {
        if (this != &other) {
            buckets = std::move(other.buckets);
            sz = other.sz;
            max_load_factor = other.max_load_factor;
            hash = std::move(other.hash);
            key_equal = std::move(other.key_equal);
            other.sz = 0;
        }
        return *this;
    }
    unordered_map& operator=(std::initializer_list<value_type> init) {
        clear();
        for (const auto& pair : init) insert(pair);
        return *this;
    }

    // Iterators
    iterator begin() {
        for (size_t i = 0; i < buckets.size(); ++i) {
            if (!buckets[i].empty()) {
                return iterator(&buckets, i, buckets[i].begin());
            }
        }
        return end();
    }
    iterator end() { return iterator(&buckets, buckets.size(), typename bucket_type::iterator()); }

    // Capacity
    bool empty() const { return sz == 0; }
    size_t size() const { return sz; }
    size_t bucket_count() const { return buckets.size(); }
    float load_factor() const { return static_cast<float>(sz) / buckets.size(); }
    float max_load_factor() const { return max_load_factor; }
    void max_load_factor(float ml) { max_load_factor = ml; }

    // Element access
    T& operator[](const Key& key) {
        auto [it, inserted] = insert({key, T()});
        return it->second;
    }

    T& at(const Key& key) {
        auto it = find(key);
        if (it == end()) throw std::out_of_range("Key not found");
        return it->second;
    }

    const T& at(const Key& key) const {
        auto it = find(key);
        if (it == end()) throw std::out_of_range("Key not found");
        return it->second;
    }

    // Modifiers
    void clear() {
        for (auto& bucket : buckets) {
            bucket.clear();
        }
        sz = 0;
    }

    std::pair<iterator, bool> insert(const value_type& value) {
        size_t bucket_index = get_bucket_index(value.first);
        auto& bucket = buckets[bucket_index];

        for (auto it = bucket.begin(); it != bucket.end(); ++it) {
            if (key_equal(it->first, value.first)) {
                return {iterator(&buckets, bucket_index, it), false};
            }
        }

        bucket.push_front(value);
        ++sz;
        check_rehash();
        return {iterator(&buckets, bucket_index, bucket.begin()), true};
    }

    size_t erase(const Key& key) {
        size_t bucket_index = get_bucket_index(key);
        auto& bucket = buckets[bucket_index];
        auto prev = bucket.before_begin();
        auto curr = bucket.begin();

        while (curr != bucket.end()) {
            if (key_equal(curr->first, key)) {
                bucket.erase_after(prev);
                --sz;
                return 1;
            }
            prev = curr;
            ++curr;
        }
        return 0;
    }

    // Lookup
    iterator find(const Key& key) {
        size_t bucket_index = get_bucket_index(key);
        auto& bucket = buckets[bucket_index];

        for (auto it = bucket.begin(); it != bucket.end(); ++it) {
            if (key_equal(it->first, key)) {
                return iterator(&buckets, bucket_index, it);
            }
        }
        return end();
    }

    size_t count(const Key& key) const {
        size_t bucket_index = get_bucket_index(key);
        const auto& bucket = buckets[bucket_index];

        for (const auto& pair : bucket) {
            if (key_equal(pair.first, key)) {
                return 1;
            }
        }
        return 0;
    }

    bool contains(const Key& key) const {
        return count(key) > 0;
    }
};

} // namespace custom

#endif // CUSTOM_UNORDERED_MAP_H 