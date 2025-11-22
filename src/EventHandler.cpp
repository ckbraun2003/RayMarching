#include "EventHandler.hpp"
#include "imgui.h"
#include <iostream>

// Initialize static instance
EventHandler* EventHandler::instance = nullptr;

EventHandler::EventHandler(Camera& cam, float screenWidth, float screenHeight)
    : camera(cam), lastX(screenWidth / 2.0f), lastY(screenHeight / 2.0f), firstMouse(true)
{
    instance = this;
}

void EventHandler::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void EventHandler::processInput(GLFWwindow* window, float deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    // Toggle UI Mode (Tab Key)
    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) {
        if (!tabKeyPressed) {
            uiMode = !uiMode;
            tabKeyPressed = true;
            std::cout << "Switched to " << (uiMode ? "UI" : "Viewport") << " mode." << std::endl;
        }
    } else {
        tabKeyPressed = false;
    }

    // Only process camera movement if NOT in UI mode
    if (!uiMode) {
        // Manually poll mouse position for camera control (avoids callback conflicts with ImGui)
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        
        float xposf = static_cast<float>(xpos);
        float yposf = static_cast<float>(ypos);

        if (firstMouse)
        {
            lastX = xposf;
            lastY = yposf;
            firstMouse = false;
        }

        float xoffset = xposf - lastX;
        float yoffset = lastY - yposf;

        lastX = xposf;
        lastY = yposf;

        camera.processMouse(xoffset, yoffset);
        
        // Camera movement with WASD
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.moveForward(deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.moveBackward(deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.moveLeft(deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.moveRight(deltaTime);
        
        // Vertical movement
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            camera.moveUp(deltaTime);
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            camera.moveDown(deltaTime);
    }
        
    // Toggle Mode (G Key) - works in both modes
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
        if (!gKeyPressed) {
            useGpu = !useGpu;
            gKeyPressed = true;
            std::cout << "Switched to " << (useGpu ? "GPU" : "CPU") << " mode." << std::endl;
        }
    } else {
        gKeyPressed = false;
    }
}

void EventHandler::setFirstMouse(bool first)
{
    firstMouse = first;
}
