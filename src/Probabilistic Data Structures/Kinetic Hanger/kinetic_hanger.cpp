#include "kinetic_hanger.hpp"

namespace kinetic {

KineticHanger::KineticHanger() : root(nullptr), gen(rd()) {}

void KineticHanger::buildHanger(const std::vector<int>& elements) {
    root.reset();
    while (!eventQueue.empty()) eventQueue.pop();

    // Sort elements by their priorities
    std::vector<std::pair<int, int>> sortedElements;
    for (int elem : elements) {
        sortedElements.emplace_back(elem, getRandomPriority());
    }
    std::sort(sortedElements.begin(), sortedElements.end(),
              [](const auto &a, const auto &b) { return a.second > b.second; });

    for (const auto &elem : sortedElements) {
        root = hang(std::move(root), elem.first, elem.second);
    }

    // Update the event queue
    updateCertificates(root);
}

void KineticHanger::insert(int element) {
    int priority = getRandomPriority();
    root = hang(std::move(root), element, priority);
    updateCertificates(root);
}

void KineticHanger::remove(int element) {
    root = deleteNode(std::move(root), element);
    updateCertificates(root);
}

bool KineticHanger::isEmpty() const {
    return root == nullptr;
}

std::unique_ptr<Node> KineticHanger::hang(std::unique_ptr<Node> node, int element, int priority) {
    if (!node) return std::make_unique<Node>(element, priority);

    if (priority > node->priority) {
        auto newRoot = std::make_unique<Node>(element, priority);
        newRoot->left = std::move(node);
        return newRoot;
    }

    // Randomly decide to place the element in the left or right subtree
    if (gen() % 2 == 0) {
        node->left = hang(std::move(node->left), element, priority);
    } else {
        node->right = hang(std::move(node->right), element, priority);
    }
    return node;
}

std::unique_ptr<Node> KineticHanger::deleteNode(std::unique_ptr<Node> node, int element) {
    if (!node) return nullptr;

    if (node->element == element) {
        if (!node->left) return std::move(node->right);
        if (!node->right) return std::move(node->left);

        if (node->left->priority > node->right->priority) {
            node->element = node->left->element;
            node->priority = node->left->priority;
            node->left = deleteNode(std::move(node->left), node->element);
        } else {
            node->element = node->right->element;
            node->priority = node->right->priority;
            node->right = deleteNode(std::move(node->right), node->element);
        }
    } else if (gen() % 2 == 0) {
        node->left = deleteNode(std::move(node->left), element);
    } else {
        node->right = deleteNode(std::move(node->right), element);
    }
    return node;
}

void KineticHanger::updateCertificates(std::unique_ptr<Node>& node) {
    if (!node) return;

    // Clear the event queue and create new certificates for the current tree
    while (!eventQueue.empty()) eventQueue.pop();

    // Traverse the tree and create certificates based on the priorities
    std::queue<Node*> queue;
    queue.push(node.get());

    while (!queue.empty()) {
        Node* current = queue.front();
        queue.pop();

        if (current->left) {
            eventQueue.push({current->element, current->priority});
            queue.push(current->left.get());
        }
        if (current->right) {
            eventQueue.push({current->element, current->priority});
            queue.push(current->right.get());
        }
    }
}

int KineticHanger::getRandomPriority() {
    std::uniform_int_distribution<int> dist(1, 1000);
    return dist(gen);
}

} 
