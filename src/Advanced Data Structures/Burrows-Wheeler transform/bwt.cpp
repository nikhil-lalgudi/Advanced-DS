/***************************************************************************
*               Implementation for Burrows-Wheeler Transform Library
*
*   File    : bwt.cpp
*   Purpose : Provides functions to apply and reverse the Burrows-Wheeler
*             transform with optional move-to-front encoding/decoding.
*   Author  : Michael Dipperstein (Modified for C++ by [Your Name])
*   Date    : October 28, 2024
*
****************************************************************************/

#include "bwt.hpp"
#include <algorithm>
#include <numeric>  // For std::iota
#include <vector>
#include <stdexcept>

namespace bwt {

constexpr size_t BurrowsWheelerTransform::BLOCK_SIZE;
std::vector<unsigned char> BurrowsWheelerTransform::block(BLOCK_SIZE);

int BurrowsWheelerTransform::transform(std::ifstream& fpIn, std::ofstream& fpOut, XformMethod method) {
    if (!fpIn.is_open() || !fpOut.is_open()) {
        std::cerr << "Invalid file stream provided." << std::endl;
        return -1;
    }

    std::vector<unsigned int> rotationIdx(BLOCK_SIZE);
    std::vector<unsigned int> v(BLOCK_SIZE);
    std::vector<unsigned char> last(BLOCK_SIZE);

    unsigned int s0Idx;
    std::array<unsigned int, 256> counters{};
    std::array<unsigned int, 256> offsetTable{};

    while (fpIn.read(reinterpret_cast<char*>(block.data()), BLOCK_SIZE)) {
        size_t blockSize = fpIn.gcount();

        counters.fill(0);
        for (size_t i = 0; i < blockSize; ++i) {
            counters[block[i]]++;
        }

        offsetTable[0] = 0;
        for (size_t i = 1; i < 256; ++i) {
            offsetTable[i] = offsetTable[i - 1] + counters[i - 1];
        }

        for (size_t i = 0; i < blockSize - 1; ++i) {
            unsigned int j = block[i + 1];
            v[offsetTable[j]++] = i;
        }
        v[offsetTable[block[0]]] = blockSize - 1;

        offsetTable[0] = 0;
        for (size_t i = 1; i < 256; ++i) {
            offsetTable[i] = offsetTable[i - 1] + counters[i - 1];
        }

        for (size_t i = 0; i < blockSize; ++i) {
            unsigned int j = v[i];
            rotationIdx[offsetTable[block[j]]++] = j;
        }

        for (size_t i = 0, k = 0; (i <= 255) && (k < blockSize - 1); ++i) {
            for (size_t j = 0; (j <= 255) && (k < blockSize - 1); ++j) {
                unsigned int first = k;
                while ((i == block[rotationIdx[k]]) &&
                       (j == block[(rotationIdx[k] + 1) % blockSize])) {
                    k++;
                    if (k == blockSize) break;
                }
                if (k - first > 1) {
                    std::qsort(&rotationIdx[first], k - first, sizeof(int), comparePresorted);
                }
            }
        }

        s0Idx = 0;
        for (size_t i = 0; i < blockSize; ++i) {
            last[i] = (rotationIdx[i] != 0) ? block[rotationIdx[i] - 1] : block[blockSize - 1];
            if (rotationIdx[i] == 0) s0Idx = i;
        }

        if (method == XformMethod::WITH_MTF) {
            if (doMTF(last) != 0) return -1;
        }

        fpOut.write(reinterpret_cast<char*>(&s0Idx), sizeof(s0Idx));
        fpOut.write(reinterpret_cast<char*>(last.data()), blockSize);
    }
    return 0;
}

int BurrowsWheelerTransform::reverseTransform(std::ifstream& fpIn, std::ofstream& fpOut, XformMethod method) {
    if (!fpIn.is_open() || !fpOut.is_open()) {
        std::cerr << "Invalid file stream provided." << std::endl;
        return -1;
    }

    std::vector<int> pred(BLOCK_SIZE);
    std::vector<unsigned char> unrotated(BLOCK_SIZE);
    unsigned int s0Idx;
    std::array<int, 256> count{};

    while (fpIn.read(reinterpret_cast<char*>(&s0Idx), sizeof(s0Idx))) {
        size_t blockSize = fpIn.read(reinterpret_cast<char*>(block.data()), BLOCK_SIZE).gcount();

        if (method == XformMethod::WITH_MTF) {
            if (undoMTF(block) != 0) return -1;
        }

        std::fill(count.begin(), count.end(), 0);
        for (size_t i = 0; i < blockSize; ++i) {
            pred[i] = count[block[i]]++;
        }

        int sum = 0;
        for (int i = 0; i <= 255; ++i) {
            int tmp = count[i];
            count[i] = sum;
            sum += tmp;
        }

        size_t i = s0Idx;
        for (size_t j = blockSize; j > 0; --j) {
            unrotated[j - 1] = block[i];
            i = pred[i] + count[block[i]];
        }

        fpOut.write(reinterpret_cast<char*>(unrotated.data()), blockSize);
    }
    return 0;
}

int BurrowsWheelerTransform::doMTF(std::vector<unsigned char>& last) {
    std::vector<unsigned char> list(256);
    std::iota(list.begin(), list.end(), 0);

    for (size_t i = 0; i < last.size(); ++i) {
        auto it = std::find(list.begin(), list.end(), last[i]);
        int idx = std::distance(list.begin(), it);
        last[i] = static_cast<unsigned char>(idx);
        list.erase(it);
        list.insert(list.begin(), last[i]);
    }
    return 0;
}

int BurrowsWheelerTransform::undoMTF(std::vector<unsigned char>& last) {
    std::vector<unsigned char> list(256);
    std::iota(list.begin(), list.end(), 0);

    for (size_t i = 0; i < last.size(); ++i) {
        unsigned char ch = list[last[i]];
        last[i] = ch;
        list.erase(list.begin() + last[i]);
        list.insert(list.begin(), ch);
    }
    return 0;
}

int BurrowsWheelerTransform::comparePresorted(const void* s1, const void* s2) {
    unsigned int offset1 = *reinterpret_cast<const unsigned int*>(s1) + 2;
    unsigned int offset2 = *reinterpret_cast<const unsigned int*>(s2) + 2;

    for (size_t i = 2; i < block.size(); ++i) {
        unsigned char c1 = block[offset1 % block.size()];
        unsigned char c2 = block[offset2 % block.size()];

        if (c1 != c2) return (c1 > c2) ? 1 : -1;
        offset1++;
        offset2++;
    }
    return 0;
}

} // namespace bwt