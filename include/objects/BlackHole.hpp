#pragma once
#include "Object.hpp"
#include <glm/glm.hpp>

class BlackHole : public Object {
public:
    float mass;
    float rs; // Schwarzschild Radius
    float diskInner;
    float diskOuter;

    BlackHole(glm::vec3 pos, float m, float dInner = 0.0f, float dOuter = 0.0f) 
        : Object(pos), mass(m), diskInner(dInner), diskOuter(dOuter) {
        rs = 2.0f * mass;
        
        // Default disk values if not provided but mass is
        if (dInner == 0.0f && dOuter == 0.0f) {
            diskInner = 3.0f * rs;
            diskOuter = 9.0f * rs;
        }
    }
};
