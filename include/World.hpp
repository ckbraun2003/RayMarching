#pragma once
#include <vector>
#include <memory>
#include "objects/Object.hpp"

class World {
public:
    std::vector<std::shared_ptr<Object>> objects;

    void add(std::shared_ptr<Object> obj);
    void clear();

    // Helper to find the first object of a specific type (e.g., BlackHole)
    template <typename T>
    std::shared_ptr<T> getFirst() const {
        for (const auto& obj : objects) {
            if (auto casted = std::dynamic_pointer_cast<T>(obj)) {
                return casted;
            }
        }
        return nullptr;
    }
};
