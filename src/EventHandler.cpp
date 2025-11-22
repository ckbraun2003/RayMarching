#include "EventHandler.hpp"
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

void EventHandler::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    if (!instance) return;

    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (instance->firstMouse)
    {
        instance->lastX = xpos;
        instance->lastY = ypos;
        instance->firstMouse = false;
    }

    float xoffset = xpos - instance->lastX;
    float yoffset = instance->lastY - ypos; // Reversed since y-coordinates go from bottom to top

    instance->lastX = xpos;
    instance->lastY = ypos;

    instance->camera.processMouse(xoffset, yoffset);
}

void EventHandler::processInput(GLFWwindow* window, float deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

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
        
    // Toggle Mode (G Key)
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
