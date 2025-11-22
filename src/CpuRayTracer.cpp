#include "CpuRayTracer.hpp"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

// Simple shader to display the texture
const char* textureVertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = vec4(aPos, 1.0);
}
)";

const char* textureFragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{
    FragColor = texture(screenTexture, TexCoords);
}
)";

CpuRayTracer::CpuRayTracer() {}

CpuRayTracer::~CpuRayTracer() {
    glDeleteVertexArrays(1, &quadVAO);
    glDeleteBuffers(1, &quadVBO);
    glDeleteTextures(1, &textureID);
    glDeleteProgram(shaderProgram);
}

void CpuRayTracer::init(int width, int height) {
    setupQuad();
    setupShader();
    
    // Initialize texture
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    bufferWidth = width;
    bufferHeight = height;
    pixelBuffer.resize(width * height * 3);
}

void CpuRayTracer::setupQuad() {
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

void CpuRayTracer::setupShader() {
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &textureVertexShaderSource, NULL);
    glCompileShader(vertexShader);
    
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &textureFragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

glm::vec3 CpuRayTracer::traceRay(const glm::vec3& origin, const glm::vec3& direction) {
    // Sphere definition (matching GPU)
    glm::vec3 sphereCenter(0.0f, 0.0f, -3.0f);
    float sphereRadius = 1.0f;
    glm::vec3 sphereColor(1.0f, 0.2f, 0.2f);
    
    // Ray-Sphere Intersection
    glm::vec3 oc = origin - sphereCenter;
    float a = glm::dot(direction, direction);
    float b = 2.0f * glm::dot(oc, direction);
    float c = glm::dot(oc, oc) - sphereRadius * sphereRadius;
    float discriminant = b*b - 4*a*c;
    
    if (discriminant > 0) {
        float t = (-b - sqrt(discriminant)) / (2.0f * a);
        if (t > 0) {
            // Hit
            glm::vec3 hitPoint = origin + direction * t;
            glm::vec3 normal = glm::normalize(hitPoint - sphereCenter);
            
            // Simple lighting
            glm::vec3 lightDir = glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f));
            float diff = std::max(glm::dot(normal, lightDir), 0.0f);
            glm::vec3 ambient(0.1f);
            
            return (ambient + diff) * sphereColor;
        }
    }
    
    // Background
    glm::vec3 unitDir = glm::normalize(direction);
    float t_bg = 0.5f * (unitDir.y + 1.0f);
    return (1.0f - t_bg) * glm::vec3(1.0f, 1.0f, 1.0f) + t_bg * glm::vec3(0.5f, 0.7f, 1.0f);
}

void CpuRayTracer::render(const Camera& camera, int width, int height) {
    // Resize buffer if needed
    if (width != bufferWidth || height != bufferHeight) {
        bufferWidth = width;
        bufferHeight = height;
        pixelBuffer.resize(width * height * 3);
        
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, NULL);
    }
    
    // Calculate camera basis
    glm::vec3 w = glm::normalize(camera.position - (camera.position + camera.front));
    glm::vec3 u = glm::normalize(glm::cross(camera.up, w));
    glm::vec3 v = glm::cross(w, u);
    
    float theta = glm::radians((float)camera.zoom);
    float h = tan(theta/2);
    float viewport_height = 2.0 * h;
    float viewport_width = viewport_height * ((float)width/height);
    
    glm::vec3 horizontal = viewport_width * u;
    glm::vec3 vertical = viewport_height * v;
    glm::vec3 lower_left_corner = camera.position - horizontal/2.0f - vertical/2.0f - w;
    
    // Parallelize this loop for performance if possible (OpenMP would be good here, but standard C++ for now)
    #pragma omp parallel for
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            float u_coord = (float)i / (width - 1);
            float v_coord = (float)j / (height - 1);
            
            glm::vec3 rayDir = lower_left_corner + u_coord*horizontal + v_coord*vertical - camera.position;
            glm::vec3 color = traceRay(camera.position, rayDir);
            
            int index = (j * width + i) * 3;
            pixelBuffer[index] = color.r;
            pixelBuffer[index + 1] = color.g;
            pixelBuffer[index + 2] = color.b;
        }
    }
    
    // Update texture
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB, GL_FLOAT, pixelBuffer.data());
    
    // Render quad
    glUseProgram(shaderProgram);
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}
