#include "randombin.hpp"

// Since this is a template class, we need to include the implementation in a header file
// This file serves as a placeholder for non-template methods or explicit template instantiations

// Example of explicit instantiation for common types
template class RandomBinaryTree<int>;
template class RandomBinaryTree<double>;
template class RandomBinaryTree<std::string>; 