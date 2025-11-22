#include "UIManager.hpp"
#include "Camera.hpp"
#include "imgui.h"
#include "imgui_internal.h"
#include <algorithm>

UIManager::UIManager() {
}

UIManager::~UIManager() {
}

void UIManager::init() {
    setupStyle();
}

void UIManager::setupStyle() {
    ImGuiStyle& style = ImGui::GetStyle();
    
    // Blender-inspired dark theme
    ImVec4* colors = style.Colors;
    colors[ImGuiCol_Text]                   = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
    colors[ImGuiCol_TextDisabled]           = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    colors[ImGuiCol_WindowBg]               = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
    colors[ImGuiCol_ChildBg]                = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
    colors[ImGuiCol_PopupBg]                = ImVec4(0.20f, 0.20f, 0.20f, 0.94f);
    colors[ImGuiCol_Border]                 = ImVec4(0.30f, 0.30f, 0.30f, 0.50f);
    colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg]                = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
    colors[ImGuiCol_FrameBgActive]          = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
    colors[ImGuiCol_TitleBg]                = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
    colors[ImGuiCol_TitleBgActive]          = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    colors[ImGuiCol_MenuBarBg]              = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
    colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    colors[ImGuiCol_CheckMark]              = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_SliderGrab]             = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
    colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.37f, 0.61f, 1.00f, 1.00f);
    colors[ImGuiCol_Button]                 = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
    colors[ImGuiCol_ButtonHovered]          = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
    colors[ImGuiCol_ButtonActive]           = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_Header]                 = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
    colors[ImGuiCol_HeaderHovered]          = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    colors[ImGuiCol_HeaderActive]           = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_Separator]              = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.26f, 0.59f, 0.98f, 0.78f);
    colors[ImGuiCol_SeparatorActive]        = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_ResizeGrip]             = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
    colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    colors[ImGuiCol_Tab]                    = ImVec4(0.18f, 0.35f, 0.58f, 0.86f);
    colors[ImGuiCol_TabHovered]             = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    colors[ImGuiCol_TabActive]              = ImVec4(0.20f, 0.41f, 0.68f, 1.00f);
    colors[ImGuiCol_TabUnfocused]           = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
    colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
    colors[ImGuiCol_DockingPreview]         = ImVec4(0.26f, 0.59f, 0.98f, 0.70f);
    colors[ImGuiCol_DockingEmptyBg]         = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_PlotLines]              = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram]          = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    colors[ImGuiCol_DragDropTarget]         = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    colors[ImGuiCol_NavHighlight]           = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

    // Style tweaks
    style.WindowRounding = 4.0f;
    style.FrameRounding = 3.0f;
    style.GrabRounding = 3.0f;
    style.TabRounding = 4.0f;
    style.WindowPadding = ImVec2(8, 8);
    style.FramePadding = ImVec2(5, 3);
    style.ItemSpacing = ImVec2(8, 6);
    style.ItemInnerSpacing = ImVec2(6, 4);
    style.IndentSpacing = 20.0f;
    style.ScrollbarSize = 14.0f;
    style.ScrollbarRounding = 9.0f;
    style.GrabMinSize = 10.0f;
}

void UIManager::beginFrame() {
    // This is typically called by ImGui_ImplXXX_NewFrame() in main.cpp
}

void UIManager::render(float deltaTime, float fps, unsigned int viewportTextureID, int texWidth, int texHeight) {
    // Update performance tracking
    updateFrameTime(deltaTime);

    // Render dockspace and all panels
    renderDockspace();
    renderViewport(viewportTextureID, texWidth, texHeight);
    renderRenderSettingsPanel();
    renderCameraSettingsPanel();
    renderSceneSettingsPanel();
    renderPerformancePanel(fps);
    renderInfoBar();
}

void UIManager::endFrame() {
    // This is typically called by ImGui::Render() in main.cpp
}

void UIManager::renderDockspace() {
    // Create a fullscreen dockspace
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);
    
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse;
    window_flags |= ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    
    ImGui::Begin("DockSpace", nullptr, window_flags);
    ImGui::PopStyleVar(3);

    // Create DockSpace
    ImGuiID dockspace_id = ImGui::GetID("MainDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

    // Apply layout on first frame
    if (firstFrame) {
        applyDockingLayout(dockspace_id);
        firstFrame = false;
    }

    // Menu bar
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Exit")) {
                // Signal to close application (needs to be handled in main.cpp)
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View")) {
            ImGui::MenuItem("Render Settings", nullptr, nullptr);
            ImGui::MenuItem("Camera Settings", nullptr, nullptr);
            ImGui::MenuItem("Scene Settings", nullptr, nullptr);
            ImGui::MenuItem("Performance", nullptr, nullptr);
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    ImGui::End();
}

void UIManager::applyDockingLayout(ImGuiID dockspaceId) {
    // Clear any existing layout
    ImGui::DockBuilderRemoveNode(dockspaceId);
    ImGui::DockBuilderAddNode(dockspaceId, ImGuiDockNodeFlags_DockSpace);
    ImGui::DockBuilderSetNodeSize(dockspaceId, ImGui::GetMainViewport()->Size);

    // Split the dockspace into regions
    ImGuiID dock_id_left, dock_id_right, dock_id_center;
    
    dock_id_left = ImGui::DockBuilderSplitNode(dockspaceId, ImGuiDir_Left, 0.20f, nullptr, &dock_id_center);
    dock_id_right = ImGui::DockBuilderSplitNode(dock_id_center, ImGuiDir_Right, 0.25f, nullptr, &dock_id_center);

    // Dock windows to their positions
    ImGui::DockBuilderDockWindow("Viewport", dock_id_center);
    ImGui::DockBuilderDockWindow("Render Settings", dock_id_left);
    ImGui::DockBuilderDockWindow("Scene Settings", dock_id_left);
    ImGui::DockBuilderDockWindow("Camera Settings", dock_id_right);
    ImGui::DockBuilderDockWindow("Performance", dock_id_right);

    ImGui::DockBuilderFinish(dockspaceId);
}

void UIManager::renderViewport(unsigned int textureID, int texWidth, int texHeight) {
    ImGui::Begin("Viewport");
    
    // Display mode indicator
    ImGui::TextColored(uiMode ? ImVec4(0.2f, 0.8f, 0.2f, 1.0f) : ImVec4(0.8f, 0.8f, 0.2f, 1.0f),
                      uiMode ? "UI Mode (Tab to toggle)" : "Viewport Mode (Tab to toggle)");
    
    ImGui::Separator();
    
    // Get available content region
    ImVec2 viewportSize = ImGui::GetContentRegionAvail();
    
    // Display the rendered texture
    if (textureID != 0 && texWidth > 0 && texHeight > 0) {
        // Calculate aspect ratio preserving display
        float aspectRatio = (float)texWidth / (float)texHeight;
        ImVec2 imageSize = viewportSize;
        
        if (viewportSize.x / viewportSize.y > aspectRatio) {
            imageSize.x = viewportSize.y * aspectRatio;
        } else {
            imageSize.y = viewportSize.x / aspectRatio;
        }
        
        // Center the image
        ImVec2 cursorPos = ImGui::GetCursorPos();
        ImVec2 offset = ImVec2(
            (viewportSize.x - imageSize.x) * 0.5f,
            (viewportSize.y - imageSize.y) * 0.5f
        );
        ImGui::SetCursorPos(ImVec2(cursorPos.x + offset.x, cursorPos.y + offset.y));
        
        ImGui::Image((void*)(intptr_t)textureID, imageSize, ImVec2(0, 1), ImVec2(1, 0));
    } else {
        ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), "No viewport texture available");
    }
    
    ImGui::End();
}

void UIManager::renderRenderSettingsPanel() {
    ImGui::Begin("Render Settings");

    if (ImGui::CollapsingHeader("Resolution", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::DragInt("Width", &renderSettings.width, 1.0f, 640, 3840);
        ImGui::DragInt("Height", &renderSettings.height, 1.0f, 480, 2160);
        
        if (ImGui::Button("720p")) { renderSettings.width = 1280; renderSettings.height = 720; }
        ImGui::SameLine();
        if (ImGui::Button("1080p")) { renderSettings.width = 1920; renderSettings.height = 1080; }
        ImGui::SameLine();
        if (ImGui::Button("1440p")) { renderSettings.width = 2560; renderSettings.height = 1440; }
    }

    if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::SliderFloat("FOV", &renderSettings.fov, 20.0f, 120.0f, "%.1f");
    }

    if (ImGui::CollapsingHeader("Render Mode", ImGuiTreeNodeFlags_DefaultOpen)) {
        bool gpuMode = renderSettings.useGpu;
        if (ImGui::Checkbox("Use GPU", &gpuMode)) {
            renderSettings.useGpu = gpuMode;
        }
        ImGui::SameLine();
        ImGui::TextDisabled("(G to toggle)");
    }

    if (ImGui::CollapsingHeader("Ray Tracing", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::SliderInt("Max Steps", &renderSettings.maxRaySteps, 50, 500);
        ImGui::SliderFloat("Max Distance", &renderSettings.maxDistance, 1000.0f, 100000.0f, "%.0f");
        ImGui::SliderFloat("Adaptive Step", &renderSettings.adaptiveStepSize, 0.01f, 0.2f, "%.3f");
        ImGui::SliderFloat("Bending Strength", &renderSettings.bendingStrength, 0.1f, 5.0f, "%.2f");
    }

    ImGui::End();
}

void UIManager::renderCameraSettingsPanel() {
    ImGui::Begin("Camera Settings");

    if (ImGui::CollapsingHeader("Position", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::DragFloat3("Position", &cameraSettings.position.x, 0.1f);
        ImGui::DragFloat("Yaw", &cameraSettings.yaw, 1.0f, -180.0f, 180.0f);
        ImGui::DragFloat("Pitch", &cameraSettings.pitch, 1.0f, -89.0f, 89.0f);
        
        if (ImGui::Button("Reset Position")) {
            cameraSettings.position = glm::vec3(0.0f, 0.0f, 3.0f);
            cameraSettings.yaw = -90.0f;
            cameraSettings.pitch = 0.0f;
        }
    }

    if (ImGui::CollapsingHeader("Movement", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::SliderFloat("Speed", &cameraSettings.movementSpeed, 0.1f, 20.0f, "%.1f");
        ImGui::SliderFloat("Mouse Sensitivity", &cameraSettings.mouseSensitivity, 0.01f, 1.0f, "%.3f");
        ImGui::Checkbox("Constrain Pitch", &cameraSettings.constrainPitch);
    }

    if (ImGui::CollapsingHeader("Controls Help")) {
        ImGui::TextWrapped("WASD - Move horizontally");
        ImGui::TextWrapped("Space - Move up");
        ImGui::TextWrapped("Shift - Move down");
        ImGui::TextWrapped("Mouse - Look around");
        ImGui::TextWrapped("Tab - Toggle UI/Viewport mode");
        ImGui::TextWrapped("G - Toggle GPU/CPU rendering");
    }

    ImGui::End();
}

void UIManager::renderSceneSettingsPanel() {
    ImGui::Begin("Scene Settings");

    if (ImGui::CollapsingHeader("Background", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::ColorEdit3("Background Color", &sceneSettings.backgroundColor.x);
        ImGui::Checkbox("Show Starfield", &sceneSettings.showStarfield);
    }

    if (ImGui::CollapsingHeader("Starfield", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::SliderFloat("Density", &sceneSettings.starfieldDensity, 0.990f, 0.999f, "%.4f");
        ImGui::SliderFloat("Nebula Intensity", &sceneSettings.nebulaIntensity, 0.0f, 2.0f, "%.2f");
    }

    ImGui::End();
}

void UIManager::renderPerformancePanel(float fps) {
    ImGui::Begin("Performance");

    if (ImGui::CollapsingHeader("FPS", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Text("FPS: %.1f", fps);
        ImGui::Text("Frame Time: %.3f ms", 1000.0f / fps);
        
        // Frame time graph
        ImGui::PlotLines("Frame Time",
                        perfSettings.frameTimeHistory.data(),
                        perfSettings.frameTimeHistory.size(),
                        perfSettings.frameTimeIndex,
                        nullptr,
                        0.0f,
                        33.33f,  // ~30 FPS max on graph
                        ImVec2(0, 80));
    }

    if (ImGui::CollapsingHeader("Options")) {
        ImGui::Checkbox("Show FPS", &perfSettings.showFps);
        ImGui::Checkbox("VSync", &perfSettings.vsync);
        ImGui::SliderFloat("Target FPS", &perfSettings.targetFps, 30.0f, 144.0f, "%.0f");
    }

    ImGui::End();
}

void UIManager::renderInfoBar() {
    // Could add a status bar at the bottom if desired
}

void UIManager::updateFrameTime(float deltaTime) {
    float frameTimeMs = deltaTime * 1000.0f;
    perfSettings.frameTimeHistory[perfSettings.frameTimeIndex] = frameTimeMs;
    perfSettings.frameTimeIndex = (perfSettings.frameTimeIndex + 1) % perfSettings.frameTimeHistory.size();
}
