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
    
    // Framebuffer management
    void initFramebuffer(int width, int height);
    void resizeFramebuffer(int width, int height);
    unsigned int getTextureID() const { return fboTexture; }
    
    // Shader parameter updates
    void setMaxSteps(int steps);
    void setMaxDistance(float distance);
    void setBendingStrength(float strength);

private:
    unsigned int quadVAO, quadVBO;
    unsigned int shaderProgram;
    
    // Framebuffer for rendering to texture
    unsigned int fbo = 0;
    unsigned int fboTexture = 0;
    unsigned int rbo = 0;  // Renderbuffer for depth/stencil
    int fboWidth = 0;
    int fboHeight = 0;

    void setupQuad();
    void setupShaders(const std::string& fragmentShaderPath);
    void cleanupFramebuffer();
};
