#include "kinetic_heater.hpp"

namespace kinetic {

KineticHeater::KineticHeater() : root(nullptr), gen(rd()) {}

void KineticHeater::buildHeater(const std::vector<int>& keys) {
    root.reset();
    while (!eventQueue.empty()) eventQueue.pop();

    // Insert each key with a random priority to build the tree
    for (int key : keys) {
        root = insertNode(std::move(root), key, getRandomPriority());
    }

    // Update the event queue (certificates)
    updateCertificates(root);
}

void KineticHeater::insert(int key) {
    int priority = getRandomPriority();
    root = insertNode(std::move(root), key, priority);
    updateCertificates(root);
}

void KineticHeater::remove(int key) {
    root = deleteNode(std::move(root), key);
    updateCertificates(root);
}

bool KineticHeater::isEmpty() const {
    return root == nullptr;
}

std::unique_ptr<Node> KineticHeater::insertNode(std::unique_ptr<Node> node, int key, int priority) {
    if (!node) return std::make_unique<Node>(key, priority);

    if (key < node->key) {
        node->left = insertNode(std::move(node->left), key, priority);
        if (node->left->priority > node->priority) {
            node = rotateRight(std::move(node));
        }
    } else {
        node->right = insertNode(std::move(node->right), key, priority);
        if (node->right->priority > node->priority) {
            node = rotateLeft(std::move(node));
        }
    }
    return node;
}

std::unique_ptr<Node> KineticHeater::deleteNode(std::unique_ptr<Node> node, int key) {
    if (!node) return nullptr;

    if (key < node->key) {
        node->left = deleteNode(std::move(node->left), key);
    } else if (key > node->key) {
        node->right = deleteNode(std::move(node->right), key);
    } else {
        // Node to delete is found
        if (!node->left) return std::move(node->right);
        if (!node->right) return std::move(node->left);

        if (node->left->priority > node->right->priority) {
            node = rotateRight(std::move(node));
            node->right = deleteNode(std::move(node->right), key);
        } else {
            node = rotateLeft(std::move(node));
            node->left = deleteNode(std::move(node->left), key);
        }
    }
    return node;
}

std::unique_ptr<Node> KineticHeater::rotateLeft(std::unique_ptr<Node> node) {
    auto newRoot = std::move(node->right);
    node->right = std::move(newRoot->left);
    newRoot->left = std::move(node);
    return newRoot;
}

std::unique_ptr<Node> KineticHeater::rotateRight(std::unique_ptr<Node> node) {
    auto newRoot = std::move(node->left);
    node->left = std::move(newRoot->right);
    newRoot->right = std::move(node);
    return newRoot;
}

void KineticHeater::handleCertificateFailure(std::unique_ptr<Node>& node) {
    if (!node) return;

    // Check priority properties and handle rotations on failure
    if (node->left && node->left->priority > node->priority) {
        node = rotateRight(std::move(node));
    } else if (node->right && node->right->priority > node->priority) {
        node = rotateLeft(std::move(node));
    }

    // Recompute certificates after handling failure
    updateCertificates(node);
}

void KineticHeater::updateCertificates(std::unique_ptr<Node>& node) {
    if (!node) return;

    // Clear existing certificates
    while (!eventQueue.empty()) eventQueue.pop();

    // Traverse and create certificates for each edge in the tree
    std::queue<Node*> queue;
    queue.push(node.get());

    while (!queue.empty()) {
        Node* current = queue.front();
        queue.pop();

        if (current->left) {
            eventQueue.push({current->key, current->priority});
            queue.push(current->left.get());
        }
        if (current->right) {
            eventQueue.push({current->key, current->priority});
            queue.push(current->right.get());
        }
    }
}

int KineticHeater::getRandomPriority() {
    std::uniform_int_distribution<int> dist(1, 1000);
    return dist(gen);
}

} // namespace kinetic
