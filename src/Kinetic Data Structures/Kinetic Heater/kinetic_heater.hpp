#ifndef KINETIC_HEATER_HPP
#define KINETIC_HEATER_HPP

#include <iostream>
#include <memory>
#include <queue>
#include <random>
#include <vector>

namespace kinetic {

struct Certificate {
    int key;
    int priority;
    bool operator<(const Certificate &other) const {
        return priority < other.priority;
    }
};

// Node in the kinetic heater, storing both a key and a priority.
class Node {
public:
    int key;
    int priority;
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;

    Node(int k, int p)
        : key(k), priority(p), left(nullptr), right(nullptr) {}
};

// Kinetic Heater class
class KineticHeater {
public:
    KineticHeater();

    void buildHeater(const std::vector<int>& keys);
    void insert(int key);
    void remove(int key);
    bool isEmpty() const;

private:
    std::unique_ptr<Node> root;
    std::priority_queue<Certificate> eventQueue;

    std::random_device rd;
    std::mt19937 gen;

    std::unique_ptr<Node> rotateLeft(std::unique_ptr<Node> node);
    std::unique_ptr<Node> rotateRight(std::unique_ptr<Node> node);
    void handleCertificateFailure(std::unique_ptr<Node>& node);
    std::unique_ptr<Node> insertNode(std::unique_ptr<Node> node, int key, int priority);
    std::unique_ptr<Node> deleteNode(std::unique_ptr<Node> node, int key);
    void updateCertificates(std::unique_ptr<Node>& node);
    int getRandomPriority();
};

} // namespace kinetic

#endif // KINETIC_HEATER_HPP
