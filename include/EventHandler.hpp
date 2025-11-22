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
    
    // Input processing
    void processInput(GLFWwindow* window, float deltaTime);
    
    // Mode toggling
    bool isGpuMode() const { return useGpu; }
    void toggleMode() { useGpu = !useGpu; }

    // Setters
    void setFirstMouse(bool firstMouse);
    
    // UI Mode control
    bool isUIMode() const { return uiMode; }
    void setUIMode(bool mode) { uiMode = mode; }
    void toggleUIMode() { uiMode = !uiMode; }

private:
    Camera& camera;
    float lastX;
    float lastY;
    bool firstMouse;
    bool useGpu = true;
    bool gKeyPressed = false; // Debounce for toggle key
    bool uiMode = false; // false = Viewport mode, true = UI mode
    bool tabKeyPressed = false; // Debounce for UI mode toggle

    // Static instance pointer for callbacks to access member data
    static EventHandler* instance;
};
