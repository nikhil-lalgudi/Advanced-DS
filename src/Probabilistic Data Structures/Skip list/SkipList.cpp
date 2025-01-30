#include "SkipList.hpp"

template<typename T>
SkipList<T>::SkipList() 
    : current_level(1)
    , gen(rd())
    , dis(0.0, 1.0) {
    head = std::make_shared<Node>(
        std::numeric_limits<T>::lowest(),
        MAX_LEVEL
    );
}

template<typename T>
size_t SkipList<T>::random_level() const {
    size_t level = 1;
    while (dis(gen) < P && level < MAX_LEVEL) {
        level++;
    }
    return level;
}

template<typename T>
bool SkipList<T>::insert(const T& value) {
    std::vector<std::shared_ptr<Node>> update(MAX_LEVEL);
    auto current = head;
    for (int i = current_level - 1; i >= 0; i--) {
        while (current->forward[i] && current->forward[i]->value < value) {
            current = current->forward[i];
        }
        update[i] = current;
    }
    current = current->forward[0];
    if (current && current->value == value) {
        return false;
    }
    size_t new_level = random_level();
    if (new_level > current_level) {
        for (size_t i = current_level; i < new_level; i++) {
            update[i] = head;
        }
        current_level = new_level;
    }
    auto new_node = std::make_shared<Node>(value, new_level);
    for (size_t i = 0; i < new_level; i++) {
        new_node->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = new_node;
    }
    
    return true;
}

template<typename T>
bool SkipList<T>::remove(const T& value) {
    std::vector<std::shared_ptr<Node>> update(MAX_LEVEL);
    auto current = head;
    for (int i = current_level - 1; i >= 0; i--) {
        while (current->forward[i] && current->forward[i]->value < value) {
            current = current->forward[i];
        }
        update[i] = current;
    }
    current = current->forward[0];
    if (!current || current->value != value) {
        return false;
    }
    for (size_t i = 0; i < current_level; i++) {
        if (update[i]->forward[i] != current) {
            break;
        }
        update[i]->forward[i] = current->forward[i];
    }
    while (current_level > 1 && !head->forward[current_level - 1]) {
        current_level--;
    }
    
    return true;
}

template<typename T>
bool SkipList<T>::contains(const T& value) const {
    auto current = head;
    
    for (int i = current_level - 1; i >= 0; i--) {
        while (current->forward[i] && current->forward[i]->value < value) {
            current = current->forward[i];
        }
    }
    
    current = current->forward[0];
    return (current && current->value == value);
}

template<typename T>
bool SkipList<T>::empty() const {
    return head->forward[0] == nullptr;
}

template<typename T>
size_t SkipList<T>::size() const {
    size_t count = 0;
    auto current = head->forward[0];
    while (current) {
        count++;
        current = current->forward[0];
    }
    return count;
}

template<typename T>
std::optional<T> SkipList<T>::find_min() const {
    if (empty()) {
        return std::nullopt;
    }
    return head->forward[0]->value;
}

template<typename T>
std::optional<T> SkipList<T>::find_max() const {
    if (empty()) {
        return std::nullopt;
    }
    auto current = head;
    for (size_t i = current_level - 1; i != SIZE_MAX; i--) {
        while (current->forward[i]) {
            current = current->forward[i];
        }
        if (i == 0) break;
    }
    return current->value;
}

template<typename T>
std::vector<T> SkipList<T>::range(const T& start, const T& end) const {
    std::vector<T> result;
    auto current = head;
    for (int i = current_level - 1; i >= 0; i--) {
        while (current->forward[i] && current->forward[i]->value < start) {
            current = current->forward[i];
        }
    }
    current = current->forward[0];
    while (current && current->value <= end) {
        result.push_back(current->value);
        current = current->forward[0];
    }
    return result;
}