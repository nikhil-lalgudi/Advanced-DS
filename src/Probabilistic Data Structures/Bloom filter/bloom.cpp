#include "bloom.h"
#include <cmath>

// MurmurHash3 implementation for speed optimization
[[nodiscard]] inline uint64_t murmur3_64(const void* key, size_t len, uint64_t seed) noexcept {
    const uint64_t m = 0xc6a4a7935bd1e995ULL;
    const int r = 47;

    uint64_t h = seed ^ (len * m);

    const uint64_t* data = (const uint64_t*)key;
    const uint64_t* end = data + (len / 8);

    while (data != end) {
        uint64_t k = *data++;

        k *= m;
        k ^= k >> r;
        k *= m;

        h ^= k;
        h *= m;
    }

    const unsigned char* data2 = (const unsigned char*)data;

    switch (len & 7) {
        case 7: h ^= uint64_t(data2[6]) << 48; [[fallthrough]];
        case 6: h ^= uint64_t(data2[5]) << 40; [[fallthrough]];
        case 5: h ^= uint64_t(data2[4]) << 32; [[fallthrough]];
        case 4: h ^= uint64_t(data2[3]) << 24; [[fallthrough]];
        case 3: h ^= uint64_t(data2[2]) << 16; [[fallthrough]];
        case 2: h ^= uint64_t(data2[1]) << 8;  [[fallthrough]];
        case 1: h ^= uint64_t(data2[0]);
                h *= m;
    };

    h ^= h >> r;
    h *= m;
    h ^= h >> r;

    return h;
}

BloomFilter::BloomFilter(size_t expectedElements, double falsePositiveRate) {
    // Calculate optimal size and number of hash functions
    bitSize = static_cast<size_t>(-std::log(falsePositiveRate) * expectedElements / (std::log(2) * std::log(2)));
    numHashes = static_cast<size_t>(std::ceil(std::log(2) * bitSize / expectedElements));
    // Round up to nearest multiple of 64 for efficient storage
    bitSize = (bitSize + 63) & ~63ULL;
    bits.resize(bitSize / 64, 0);
    // Generate random seeds for hash functions
    std::random_device rd;
    std::mt19937_64 gen(rd());
    hashSeeds[0] = gen();
    hashSeeds[1] = gen();
}

[[nodiscard]] inline std::pair<uint64_t, uint64_t> BloomFilter::getHash(const void* data, size_t len) const noexcept {
    return {
        murmur3_64(data, len, hashSeeds[0]),
        murmur3_64(data, len, hashSeeds[1])
    };
}

[[nodiscard]] inline size_t BloomFilter::nthHash(uint64_t hash1, uint64_t hash2, size_t n) const noexcept {
    // Double hashing technique
    return (hash1 + n * hash2) % bitSize;
}

template<typename T>
void BloomFilter::insert(const T& element) noexcept {
    const auto [hash1, hash2] = getHash(&element, sizeof(T));
    
    for (size_t i = 0; i < numHashes; i++) {
        size_t bitIndex = nthHash(hash1, hash2, i);
        bits[bitIndex / 64] |= 1ULL << (bitIndex % 64);
    }
}

template<typename T>
[[nodiscard]] bool BloomFilter::contains(const T& element) const noexcept {
    const auto [hash1, hash2] = getHash(&element, sizeof(T));
    
    for (size_t i = 0; i < numHashes; i++) {
        size_t bitIndex = nthHash(hash1, hash2, i);
        if (!(bits[bitIndex / 64] & (1ULL << (bitIndex % 64)))) {
            return false;
        }
    }
    return true;
}

void BloomFilter::clear() noexcept {
    std::fill(bits.begin(), bits.end(), 0);
}