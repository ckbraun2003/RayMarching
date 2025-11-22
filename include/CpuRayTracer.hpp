#pragma once

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Camera.hpp"

class CpuRayTracer {
public:
    CpuRayTracer();
    ~CpuRayTracer();

    void init(int width, int height);
    void render(const Camera& camera, int width, int height);

private:
    unsigned int quadVAO = 0;
    unsigned int quadVBO = 0;
    unsigned int textureID = 0;
    unsigned int shaderProgram = 0;
    
    std::vector<float> pixelBuffer;
    int bufferWidth = 0;
    int bufferHeight = 0;

    void setupQuad();
    void setupShader();
    void updateTexture(int width, int height);
    glm::vec3 traceRay(const glm::vec3& origin, const glm::vec3& direction);
};
