#include <iostream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.hpp"
#include "EventHandler.hpp"
#include "GpuRayTracer.hpp"
#include "CpuRayTracer.hpp"
#include "World.hpp"
#include "objects/BlackHole.hpp"

// --- Engine Configuration ---
struct EngineConfig {
    unsigned int width = 1920;
    unsigned int height = 1080;
    float fov = 45.0f;
    bool fullScreen = true;
};

int main()
{
    EngineConfig config;

    // 1. Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // 2. Create the Window
    GLFWwindow* window = glfwCreateWindow(config.width, config.height, "Ray Tracing Engine", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // 3. Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // --- Scene Setup ---
    // Camera
    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
    camera.zoom = config.fov;

    // World
    World world;
    auto blackHole = std::make_shared<BlackHole>(glm::vec3(0.0f, -10.0f, -50.0f), 0.5f, 0.0f, 0.0f);
    world.add(blackHole);
    
    auto blackHole2 = std::make_shared<BlackHole>(glm::vec3(0.0f, -10.0f, 50.0f), 0.5f, 0.0f, 0.0f);
    world.add(blackHole2);

    // Event Handler
    EventHandler eventHandler(camera, (float)config.width, (float)config.height);

    // Register callbacks
    glfwSetFramebufferSizeCallback(window, EventHandler::framebuffer_size_callback);
    glfwSetCursorPosCallback(window, EventHandler::mouse_callback);

    // Capture and hide the mouse cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // --- Initialize Ray Tracers ---
    GpuRayTracer gpuTracer;
    gpuTracer.init("shaders/raytracer.frag");

    CpuRayTracer cpuTracer;
    cpuTracer.init(config.width, config.height);

    // Timing
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    float lastFpsTime = 0.0f;
    int frameCount = 0;

    // 4. Render Loop
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // FPS Counter
        frameCount++;
        if (currentFrame - lastFpsTime >= 1.0f)
        {
            std::string title = "Ray Tracing Engine - " + 
                                std::string(eventHandler.isGpuMode() ? "GPU" : "CPU") + 
                                " - FPS: " + std::to_string(frameCount);
            glfwSetWindowTitle(window, title.c_str());
            frameCount = 0;
            lastFpsTime = currentFrame;
        }

        // Input
        eventHandler.processInput(window, deltaTime);

        // Rendering
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (eventHandler.isGpuMode()) {
            gpuTracer.render(camera, world, config.width, config.height, currentFrame);
        } else {
            cpuTracer.render(camera, config.width, config.height);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}