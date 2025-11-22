#pragma once
#include <glm/glm.hpp>

class Object {
public:
    glm::vec3 position;

    Object(glm::vec3 pos = glm::vec3(0.0f)) : position(pos) {}
    virtual ~Object() = default;
};
