
#ifndef BWT_HPP
#define BWT_HPP

#include <fstream>
#include <vector>
#include <array>
#include <cstring>
#include <iostream>

namespace bwt {

enum class XformMethod {
    WITHOUT_MTF = 0,
    WITH_MTF = 1
};

class BurrowsWheelerTransform {
public:
    static int transform(std::ifstream& fpIn, std::ofstream& fpOut, XformMethod method);
    static int reverseTransform(std::ifstream& fpIn, std::ofstream& fpOut, XformMethod method);

private:
    static int doMTF(std::vector<unsigned char>& last);
    static int undoMTF(std::vector<unsigned char>& last);

    static int comparePresorted(const void* s1, const void* s2);

    static constexpr size_t BLOCK_SIZE = 4096;
    static inline std::vector<unsigned char> block;
};

} // namespace bwt

#endif  

// Note: This file + cpp is inspired by Michael Dipperstein's implementation in C