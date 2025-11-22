#include <gtest/gtest.h>
#include "EventHandler.hpp"
#include "Camera.hpp"
#include <GLFW/glfw3.h>

// Mock GLFW window (we don't need a real one for logic tests if we don't call GLFW functions)
// However, EventHandler::mouse_callback uses static instance, so we need to be careful.

class EventHandlerTest : public ::testing::Test {
protected:
    Camera* camera;
    EventHandler* handler;
    GLFWwindow* mockWindow;

    void SetUp() override {
        // Initialize GLFW just in case, though we might not need a full window for logic
        if (!glfwInit()) {
            // Handle error if needed, but for unit tests usually we assume env is ok
        }
        
        camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
        handler = new EventHandler(*camera, 800.0f, 600.0f);
        
        // We can pass nullptr as window since our callback implementation 
        // currently doesn't use the window pointer for logic, only for state.
        mockWindow = nullptr; 
    }

    void TearDown() override {
        delete handler;
        delete camera;
        glfwTerminate();
    }
};

TEST_F(EventHandlerTest, InitialState) {
    // We can't easily access private members of EventHandler without friend class or getters.
    // But we can test the side effects on the camera.
    
    // Simulate first mouse movement
    handler->mouse_callback(mockWindow, 400.0, 300.0);
    
    // Camera shouldn't move on first frame (it just sets lastX/lastY)
    EXPECT_EQ(camera->yaw, -90.0f);
    EXPECT_EQ(camera->pitch, 0.0f);
}

TEST_F(EventHandlerTest, MouseMovement) {
    // First call to set initial position
    handler->mouse_callback(mockWindow, 400.0, 300.0);
    
    // Move mouse right (increase X) -> should increase yaw
    handler->mouse_callback(mockWindow, 500.0, 300.0);
    
    EXPECT_GT(camera->yaw, -90.0f);
    
    // Move mouse up (decrease Y) -> should increase pitch
    handler->mouse_callback(mockWindow, 500.0, 200.0);
    
    EXPECT_GT(camera->pitch, 0.0f);
}
