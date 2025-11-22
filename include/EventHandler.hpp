#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Camera.hpp"

class EventHandler {
public:
    // Constructor
    EventHandler(Camera& camera, float screenWidth, float screenHeight);

    // Callbacks
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    
    // Input processing
    void processInput(GLFWwindow* window, float deltaTime);
    
    // Mode toggling
    bool isGpuMode() const { return useGpu; }
    void toggleMode() { useGpu = !useGpu; }

    // Setters
    void setFirstMouse(bool firstMouse);

private:
    Camera& camera;
    float lastX;
    float lastY;
    bool firstMouse;
    bool useGpu = true;
    bool gKeyPressed = false; // Debounce for toggle key

    // Static instance pointer for callbacks to access member data
    static EventHandler* instance;
};
