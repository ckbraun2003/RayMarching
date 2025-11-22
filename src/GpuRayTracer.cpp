#include "GpuRayTracer.hpp"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include "objects/BlackHole.hpp"
#include "World.hpp"

// Helper to load shader code from file
std::string loadShaderSource(const char* filePath) {
    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);

    if (!fileStream.is_open()) {
        std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
        return "";
    }

    std::stringstream sstr;
    sstr << fileStream.rdbuf();
    content = sstr.str();
    fileStream.close();
    return content;
}

GpuRayTracer::GpuRayTracer() {}

GpuRayTracer::~GpuRayTracer() {
    cleanupFramebuffer();
    glDeleteVertexArrays(1, &quadVAO);
    glDeleteBuffers(1, &quadVBO);
    glDeleteProgram(shaderProgram);
}

void GpuRayTracer::init(const std::string& fragmentShaderPath) {
    setupQuad();
    setupShaders(fragmentShaderPath);
}

void GpuRayTracer::setupQuad() {
    float quadVertices[] = {
        // positions        // texture Coords
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
         1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };
    
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
}

void GpuRayTracer::setupShaders(const std::string& fragmentShaderPath) {
    std::string vertexCode = loadShaderSource("shaders/raytracer.vert");
    std::string fragmentCode = loadShaderSource(fragmentShaderPath.c_str());
    
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vShaderCode, NULL);
    glCompileShader(vertexShader);
    
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::GPU_RAYTRACER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
    glCompileShader(fragmentShader);
    
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::GPU_RAYTRACER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::GPU_RAYTRACER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void GpuRayTracer::render(const Camera& camera, const World& world, int width, int height, float time) {
    // Bind framebuffer if it exists
    if (fbo != 0) {
        // Resize if needed
        if (fboWidth != width || fboHeight != height) {
            resizeFramebuffer(width, height);
        }
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glViewport(0, 0, width, height);
    }
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glUseProgram(shaderProgram);
    glBindVertexArray(quadVAO);
    
    glUniform3fv(glGetUniformLocation(shaderProgram, "cameraPos"), 1, glm::value_ptr(camera.position));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(camera.getViewMatrix()));
    
    glm::mat4 projection = glm::perspective(glm::radians((float)camera.zoom), (float)width / (float)height, 0.1f, 100000.0f);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    
    glUniform1f(glGetUniformLocation(shaderProgram, "time"), time);
    
    // --- World Objects ---
    int numBlackHoles = 0;
    for (const auto& obj : world.objects) {
        if (auto bh = std::dynamic_pointer_cast<BlackHole>(obj)) {
            if (numBlackHoles < 4) {
                std::string base = "uBlackHoles[" + std::to_string(numBlackHoles) + "]";
                glUniform3fv(glGetUniformLocation(shaderProgram, (base + ".pos").c_str()), 1, glm::value_ptr(bh->position));
                glUniform1f(glGetUniformLocation(shaderProgram, (base + ".rs").c_str()), bh->rs);
                glUniform1f(glGetUniformLocation(shaderProgram, (base + ".diskInner").c_str()), bh->diskInner);
                glUniform1f(glGetUniformLocation(shaderProgram, (base + ".diskOuter").c_str()), bh->diskOuter);
                numBlackHoles++;
            }
        }
    }
    glUniform1i(glGetUniformLocation(shaderProgram, "uNumBlackHoles"), numBlackHoles);
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
    
    // Unbind framebuffer
    if (fbo != 0) {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}

void GpuRayTracer::initFramebuffer(int width, int height) {
    cleanupFramebuffer();
    
    fboWidth = width;
    fboHeight = height;
    
    // Create framebuffer
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    
    // Create texture for color attachment
    glGenTextures(1, &fboTexture);
    glBindTexture(GL_TEXTURE_2D, fboTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTexture, 0);
    
    // Create renderbuffer for depth/stencil (optional, but good practice)
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
    
    // Check framebuffer completeness
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GpuRayTracer::resizeFramebuffer(int width, int height) {
    if (width <= 0 || height <= 0) return;
    initFramebuffer(width, height);
}

void GpuRayTracer::cleanupFramebuffer() {
    if (fbo != 0) {
        glDeleteFramebuffers(1, &fbo);
        fbo = 0;
    }
    if (fboTexture != 0) {
        glDeleteTextures(1, &fboTexture);
        fboTexture = 0;
    }
    if (rbo != 0) {
        glDeleteRenderbuffers(1, &rbo);
        rbo = 0;
    }
}

void GpuRayTracer::setMaxSteps(int steps) {
    glUseProgram(shaderProgram);
    // Note: This would require modifying the shader to use a uniform instead of #define
    // For now, this is a placeholder - we'd need to update the shader
}

void GpuRayTracer::setMaxDistance(float distance) {
    glUseProgram(shaderProgram);
    // Similar to above - placeholder for future shader updates
}

void GpuRayTracer::setBendingStrength(float strength) {
    glUseProgram(shaderProgram);
    // This one we can implement since bending strength is already in the shader logic
    // We'd need to pass it as a uniform
}
