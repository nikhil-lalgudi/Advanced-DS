#ifndef KINETIC_HANGER_HPP
#define KINETIC_HANGER_HPP

#include <iostream>
#include <vector>
#include <queue>
#include <memory>
#include <random>
#include <algorithm>

namespace kinetic {

// Certificate structure representing failure conditions
struct Certificate {
    int element;   // The element associated with the certificate
    int priority;  // Priority of the element
    bool operator<(const Certificate &other) const {
        return priority < other.priority;
    }
};

// Node of the kinetic hanger tree
class Node {
public:
    int element;
    int priority;
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;

    Node(int elem, int pri)
        : element(elem), priority(pri), left(nullptr), right(nullptr) {}
};

// Kinetic Hanger class
class KineticHanger {
public:
    KineticHanger();

    void buildHanger(const std::vector<int>& elements);
    void insert(int element);
    void remove(int element);
    bool isEmpty() const;

private:
    std::unique_ptr<Node> root;
    std::priority_queue<Certificate> eventQueue;

    std::random_device rd;
    std::mt19937 gen;

    std::unique_ptr<Node> hang(std::unique_ptr<Node> node, int element, int priority);
    std::unique_ptr<Node> deleteNode(std::unique_ptr<Node> node, int element);
    void updateCertificates(std::unique_ptr<Node>& node);
    int getRandomPriority();
};

} // namespace kinetic

#endif // KINETIC_HANGER_HPP
