#include <iostream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Camera.hpp"
#include "EventHandler.hpp"
#include "GpuRayTracer.hpp"
#include "CpuRayTracer.hpp"
#include "World.hpp"
#include "objects/BlackHole.hpp"
#include "UIManager.hpp"
int main()
{
    // 1. Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    // 2. Create the Window
    GLFWwindow* window = glfwCreateWindow(1920, 1080, "Ray Tracing Engine", NULL, NULL);
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
    // --- ImGui Setup ---
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;  // Enable Docking
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    // --- UI Manager Setup ---
    UIManager uiManager;
    uiManager.init();
    // --- Scene Setup ---
    // Camera - initialize with UI settings
    Camera camera(uiManager.getCameraSettings().position);
    camera.yaw = uiManager.getCameraSettings().yaw;
    camera.pitch = uiManager.getCameraSettings().pitch;
    camera.movementSpeed = uiManager.getCameraSettings().movementSpeed;
    camera.mouseSensitivity = uiManager.getCameraSettings().mouseSensitivity;
    camera.zoom = uiManager.getRenderSettings().fov;
    // World
    World world;
    auto blackHole = std::make_shared<BlackHole>(glm::vec3(0.0f, -10.0f, -50.0f), 0.5f, 0.0f, 0.0f);
    world.add(blackHole);
    // Event Handler
    EventHandler eventHandler(camera, 1920.0f, 1080.0f);
    // Register callbacks - DON'T override ImGui's cursor callback
    glfwSetFramebufferSizeCallback(window, EventHandler::framebuffer_size_callback);
    // Note: ImGui_ImplGlfw_InitForOpenGL already set the cursor callback - don't override it!
    // Cursor starts in viewport mode (disabled)
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // --- Initialize Ray Tracers ---
    GpuRayTracer gpuTracer;
    gpuTracer.init("shaders/raytracer.frag");
    gpuTracer.initFramebuffer(uiManager.getRenderSettings().width, 
                             uiManager.getRenderSettings().height);
    CpuRayTracer cpuTracer;
    cpuTracer.init(uiManager.getRenderSettings().width, 
                   uiManager.getRenderSettings().height);
    // Timing
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    float lastFpsTime = 0.0f;
    int frameCount = 0;
    float currentFps = 60.0f;
    // Previous UI mode state for cursor management
    bool previousUIMode = false;
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
            currentFps = frameCount / (currentFrame - lastFpsTime);
            std::string title = "Ray Tracing Engine - " + 
                                std::string(eventHandler.isGpuMode() ? "GPU" : "CPU") + 
                                " - FPS: " + std::to_string((int)currentFps);
            glfwSetWindowTitle(window, title.c_str());
            frameCount = 0;
            lastFpsTime = currentFrame;
        }
        // Input
        eventHandler.processInput(window, deltaTime);
        // Sync UI mode between EventHandler and UIManager
        uiManager.setUIMode(eventHandler.isUIMode());
        // Handle cursor visibility based on UI mode
        if (eventHandler.isUIMode() != previousUIMode) {
            if (eventHandler.isUIMode()) {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                eventHandler.setFirstMouse(true); // Reset mouse on mode change
            } else {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                eventHandler.setFirstMouse(true); // Reset mouse on mode change
            }
            previousUIMode = eventHandler.isUIMode();
        }
        // Sync render mode between UI and EventHandler
        if (uiManager.getRenderSettings().useGpu != eventHandler.isGpuMode()) {
            uiManager.getRenderSettings().useGpu = eventHandler.isGpuMode();
        }
        // Apply UI camera settings to camera (only position/rotation, not during active movement)
        if (eventHandler.isUIMode()) {
            // In UI mode, apply settings from UI to camera
            camera.setPosition(uiManager.getCameraSettings().position);
            camera.setYaw(uiManager.getCameraSettings().yaw);
            camera.setPitch(uiManager.getCameraSettings().pitch);
            camera.setMovementSpeed(uiManager.getCameraSettings().movementSpeed);
            camera.setMouseSensitivity(uiManager.getCameraSettings().mouseSensitivity);
        } else {
            // In viewport mode, update UI with camera's current state
            uiManager.getCameraSettings().position = camera.position;
            uiManager.getCameraSettings().yaw = camera.yaw;
            uiManager.getCameraSettings().pitch = camera.pitch;
        }
        // Apply FOV from UI
        camera.zoom = uiManager.getRenderSettings().fov;
        // Start ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        // Render scene to framebuffer texture
        auto& renderSettings = uiManager.getRenderSettings();
        if (eventHandler.isGpuMode()) {
            gpuTracer.render(camera, world, renderSettings.width, renderSettings.height, currentFrame);
        } else {
            cpuTracer.render(camera, renderSettings.width, renderSettings.height);
        }
        // Render UI with viewport texture
        unsigned int viewportTexture = eventHandler.isGpuMode() ? 
                                       gpuTracer.getTextureID() : 0; // CPU mode doesn't have texture yet
        
        uiManager.render(deltaTime, currentFps, viewportTexture, 
                        renderSettings.width, renderSettings.height);
        // Final rendering
        glBindFramebuffer(GL_FRAMEBUFFER, 0); // Render to screen
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}