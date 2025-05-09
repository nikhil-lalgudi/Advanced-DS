#include "bloom_filter.hpp"
#include <cmath>
#include <random>

// MurmurHash3 implementation
[[nodiscard]] uint64_t BloomFilter::murmur_hash3_64(const void* key, size_t len, uint64_t seed) noexcept {
    const uint64_t m = 0xc6a4a7935bd1e995ULL;
    const int r = 47;

    uint64_t h = seed ^ (len * m);

    const uint64_t* data = static_cast<const uint64_t*>(key);
    const uint64_t* end = data + (len / 8);

    while (data != end) {
        uint64_t k = *data++;

        k *= m;
        k ^= k >> r;
        k *= m;

        h ^= k;
        h *= m;
    }

    const unsigned char* data2 = reinterpret_cast<const unsigned char*>(data);

    switch (len & 7) {
        case 7: h ^= static_cast<uint64_t>(data2[6]) << 48; [[fallthrough]];
        case 6: h ^= static_cast<uint64_t>(data2[5]) << 40; [[fallthrough]];
        case 5: h ^= static_cast<uint64_t>(data2[4]) << 32; [[fallthrough]];
        case 4: h ^= static_cast<uint64_t>(data2[3]) << 24; [[fallthrough]];
        case 3: h ^= static_cast<uint64_t>(data2[2]) << 16; [[fallthrough]];
        case 2: h ^= static_cast<uint64_t>(data2[1]) << 8;  [[fallthrough]];
        case 1: h ^= static_cast<uint64_t>(data2[0]);
                h *= m;
    }

    h ^= h >> r;
    h *= m;
    h ^= h >> r;

    return h;
}

BloomFilter::BloomFilter(size_t expected_elements, double false_positive_rate) {
    // Calculate optimal size and number of hash functions
    bit_size_ = static_cast<size_t>(-std::log(false_positive_rate) * expected_elements / (std::log(2) * std::log(2)));
    hash_count_ = static_cast<size_t>(std::ceil(std::log(2) * bit_size_ / expected_elements));
    
    // Round up to nearest multiple of 8 for byte alignment
    bit_size_ = (bit_size_ + 7) & ~7ULL;
    bits_.resize(bit_size_ / 8, 0);

    // Generate random seeds for hash functions
    std::random_device rd;
    std::mt19937_64 gen(rd());
    hash_seeds_[0] = gen();
    hash_seeds_[1] = gen();
}

template<typename T>
[[nodiscard]] std::pair<uint64_t, uint64_t> BloomFilter::get_hash(const T& element) const noexcept {
    return {
        murmur_hash3_64(&element, sizeof(T), hash_seeds_[0]),
        murmur_hash3_64(&element, sizeof(T), hash_seeds_[1])
    };
}

[[nodiscard]] size_t BloomFilter::nth_hash(uint64_t hash1, uint64_t hash2, size_t n) const noexcept {
    return (hash1 + n * hash2) % bit_size_;
}

template<typename T>
void BloomFilter::insert(const T& element) noexcept {
    const auto [hash1, hash2] = get_hash(element);
    
    for (size_t i = 0; i < hash_count_; ++i) {
        size_t bit_index = nth_hash(hash1, hash2, i);
        size_t byte_index = bit_index / 8;
        size_t bit_offset = bit_index % 8;
        bits_[byte_index] |= bit_mask[bit_offset];
    }
}

template<typename T>
[[nodiscard]] bool BloomFilter::contains(const T& element) const noexcept {
    const auto [hash1, hash2] = get_hash(element);
    
    for (size_t i = 0; i < hash_count_; ++i) {
        size_t bit_index = nth_hash(hash1, hash2, i);
        size_t byte_index = bit_index / 8;
        size_t bit_offset = bit_index % 8;
        if (!(bits_[byte_index] & bit_mask[bit_offset])) {
            return false;
        }
    }
    return true;
}

void BloomFilter::clear() noexcept {
    std::fill(bits_.begin(), bits_.end(), 0);
}

// Explicit template instantiations for common types
template void BloomFilter::insert<int>(const int&) noexcept;
template void BloomFilter::insert<long>(const long&) noexcept;
template void BloomFilter::insert<long long>(const long long&) noexcept;
template void BloomFilter::insert<std::string_view>(const std::string_view&) noexcept;

template [[nodiscard]] bool BloomFilter::contains<int>(const int&) const noexcept;
template [[nodiscard]] bool BloomFilter::contains<long>(const long&) const noexcept;
template [[nodiscard]] bool BloomFilter::contains<long long>(const long long&) const noexcept;
template [[nodiscard]] bool BloomFilter::contains<std::string_view>(const std::string_view&) const noexcept; 