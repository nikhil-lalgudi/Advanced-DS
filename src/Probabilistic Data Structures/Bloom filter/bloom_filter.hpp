#pragma once

#include <vector>
#include <cmath>
#include <random>
#include <cstdint>
#include <string_view>
#include <type_traits>

class BloomFilter {
public:
    // Constructor with expected elements and false positive rate
    explicit BloomFilter(size_t expected_elements, double false_positive_rate = 0.01);

    // Prevent copying
    BloomFilter(const BloomFilter&) = delete;
    BloomFilter& operator=(const BloomFilter&) = delete;

    // Allow moving
    BloomFilter(BloomFilter&&) noexcept = default;
    BloomFilter& operator=(BloomFilter&&) noexcept = default;

    // Core operations
    template<typename T>
    [[nodiscard]] bool contains(const T& element) const noexcept;

    template<typename T>
    void insert(const T& element) noexcept;

    // Utility functions
    [[nodiscard]] size_t size() const noexcept { return bit_size_; }
    [[nodiscard]] size_t hash_count() const noexcept { return hash_count_; }
    void clear() noexcept;

private:
    static constexpr size_t bits_per_char = 8;
    static constexpr unsigned char bit_mask[bits_per_char] = {
        0x01,  // 00000001
        0x02,  // 00000010
        0x04,  // 00000100
        0x08,  // 00001000
        0x10,  // 00010000
        0x20,  // 00100000
        0x40,  // 01000000
        0x80   // 10000000
    };

    // MurmurHash3 implementation
    [[nodiscard]] static uint64_t murmur_hash3_64(const void* key, size_t len, uint64_t seed) noexcept;

    // Double hashing implementation
    [[nodiscard]] size_t nth_hash(uint64_t hash1, uint64_t hash2, size_t n) const noexcept;

    // Get hash values for an element
    template<typename T>
    [[nodiscard]] std::pair<uint64_t, uint64_t> get_hash(const T& element) const noexcept;

    std::vector<unsigned char> bits_;
    size_t bit_size_;
    size_t hash_count_;
    uint64_t hash_seeds_[2];
}; 