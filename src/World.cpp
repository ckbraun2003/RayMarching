#include "World.hpp"

void World::add(std::shared_ptr<Object> obj) {
    objects.push_back(obj);
}

void World::clear() {
    objects.clear();
}
