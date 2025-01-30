#ifndef SKIP_LIST_HPP
#define SKIP_LIST_HPP

#include <memory>
#include <random>
#include <vector>
#include <optional>
#include <limits>

template<typename T>
class SkipList {
private:
    struct Node {
        T value;
        std::vector<std::shared_ptr<Node>> forward;
        
        explicit Node(const T& val, size_t level) 
            : value(val), forward(level, nullptr) {}
    };
    static constexpr float P = 0.5f;
    static constexpr size_t MAX_LEVEL = 16;  
    std::shared_ptr<Node> head;
    size_t current_level;
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<> dis;
    [[nodiscard]] size_t random_level() const;
    
public:
    SkipList();
    ~SkipList() = default;
    // no copying!!!
    SkipList(const SkipList&) = delete;
    SkipList& operator=(const SkipList&) = delete;
    //  moving
    SkipList(SkipList&&) = default;
    SkipList& operator=(SkipList&&) = default;
    // Core 
    [[nodiscard]] bool insert(const T& value);
    [[nodiscard]] bool remove(const T& value);
    [[nodiscard]] bool contains(const T& value) const;
    // Utility 
    [[nodiscard]] bool empty() const;
    [[nodiscard]] size_t size() const;
    [[nodiscard]] std::optional<T> find_min() const;
    [[nodiscard]] std::optional<T> find_max() const;
    // Range operations
    [[nodiscard]] std::vector<T> range(const T& start, const T& end) const;
};

#endif 