#pragma once
#include <vector>
#include <array>
#include <cstdint>
#include <bit>
#include <random>

class BloomFilter {
private:
    std::vector<uint64_t> bits;
    size_t numHashes;
    size_t bitSize;
    std::array<uint64_t, 2> hashSeeds;

    [[nodiscard]] inline std::pair<uint64_t, uint64_t> getHash(const void* data, size_t len) const noexcept;
    [[nodiscard]] inline size_t nthHash(uint64_t hash1, uint64_t hash2, size_t n) const noexcept;

public:
    BloomFilter(size_t expectedElements, double falsePositiveRate);

    [[nodiscard]] inline bool isEmpty() const noexcept { 
        return std::all_of(bits.begin(), bits.end(), [](uint64_t x) { return x == 0; });
    }

    [[nodiscard]] inline size_t size() const noexcept { return bitSize; }
    [[nodiscard]] inline size_t hashCount() const noexcept { return numHashes; }

    template<typename T>
    void insert(const T& element) noexcept;

    template<typename T>
    [[nodiscard]] bool contains(const T& element) const noexcept;

    void clear() noexcept;
};