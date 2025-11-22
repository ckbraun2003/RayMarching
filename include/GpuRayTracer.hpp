#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include "Camera.hpp"
#include "World.hpp"

class GpuRayTracer {
public:
    GpuRayTracer();
    ~GpuRayTracer();

    void init(const std::string& fragmentShaderPath);
    void render(const Camera& camera, const World& world, int width, int height, float time);

private:
    unsigned int quadVAO, quadVBO;
    unsigned int shaderProgram;

    void setupQuad();
    void setupShaders(const std::string& fragmentShaderPath);
};
