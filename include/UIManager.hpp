#pragma once

#include <glad/glad.h>
#include "imgui.h"
#include <glm/glm.hpp>
#include <string>
#include <array>

// Forward declarations
class Camera;
class GpuRayTracer;

// Centralized UI Manager for all IMGUI interface elements
class UIManager {
public:
    // --- Settings Structures ---
    struct RenderSettings {
        int width = 1920;
        int height = 1080;
        float fov = 45.0f;
        bool useGpu = true;
        int maxRaySteps = 200;
        float maxDistance = 10000.0f;
        float adaptiveStepSize = 0.08f;
        float bendingStrength = 1.5f;
    };

    struct CameraSettings {
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f);
        float yaw = -90.0f;
        float pitch = 0.0f;
        float movementSpeed = 2.5f;
        float mouseSensitivity = 0.1f;
        bool constrainPitch = true;
    };

    struct SceneSettings {
        glm::vec3 backgroundColor = glm::vec3(0.0f, 0.0f, 0.0f);
        float starfieldDensity = 0.995f;
        float nebulaIntensity = 1.0f;
        bool showStarfield = true;
    };

    struct PerformanceSettings {
        bool showFps = true;
        bool vsync = true;
        float targetFps = 60.0f;
        std::array<float, 100> frameTimeHistory = {};
        int frameTimeIndex = 0;
    };

    // --- Constructor & Destructor ---
    UIManager();
    ~UIManager();

    // --- Initialization ---
    void init();
    void setupStyle();

    // --- Main Render ---
    void beginFrame();
    void render(float deltaTime, float fps, unsigned int viewportTextureID, int texWidth, int texHeight);
    void endFrame();

    // --- Settings Access ---
    RenderSettings& getRenderSettings() { return renderSettings; }
    CameraSettings& getCameraSettings() { return cameraSettings; }
    SceneSettings& getSceneSettings() { return sceneSettings; }
    PerformanceSettings& getPerformanceSettings() { return perfSettings; }

    // --- UI Mode ---
    bool isUIMode() const { return uiMode; }
    void setUIMode(bool mode) { uiMode = mode; }
    void toggleUIMode() { uiMode = !uiMode; }

    // --- Performance Tracking ---
    void updateFrameTime(float deltaTime);

private:
    // --- Settings ---
    RenderSettings renderSettings;
    CameraSettings cameraSettings;
    SceneSettings sceneSettings;
    PerformanceSettings perfSettings;

    // --- UI State ---
    bool uiMode = false;  // false = Viewport mode, true = UI mode
    bool firstFrame = true;

    // --- Panel Rendering Methods ---
    void renderDockspace();
    void renderViewport(unsigned int textureID, int texWidth, int texHeight);
    void renderRenderSettingsPanel();
    void renderCameraSettingsPanel();
    void renderSceneSettingsPanel();
    void renderPerformancePanel(float fps);
    void renderInfoBar();

    // --- Helper Methods ---
    void applyDockingLayout(ImGuiID dockspaceId);
};
