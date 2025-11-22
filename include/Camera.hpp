#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// FPS Camera with view matrix for OpenGL rendering
class Camera {
public:
    // Camera attributes
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    // Euler angles (in degrees)
    float yaw;
    float pitch;

    // Camera options
    float movementSpeed;
    float mouseSensitivity;
    float zoom;

    // Default constructor
    Camera(
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f),
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
        float yaw = -90.0f,
        float pitch = 0.0f
    );

    // Returns the view matrix calculated using Euler angles and LookAt matrix
    glm::mat4 getViewMatrix() const;

    // Process keyboard input for camera movement
    void moveForward(float deltaTime);
    void moveBackward(float deltaTime);
    void moveLeft(float deltaTime);
    void moveRight(float deltaTime);
    void moveUp(float deltaTime);      // Move up in world Y-axis
    void moveDown(float deltaTime);    // Move down in world Y-axis

    // Process mouse movement to update camera orientation
    void processMouse(float xoffset, float yoffset, bool constrainPitch = true);
    
    // Setters for UI control
    void setPosition(const glm::vec3& pos) { position = pos; }
    void setYaw(float y) { yaw = y; updateCameraVectors(); }
    void setPitch(float p) { pitch = p; updateCameraVectors(); }
    void setMovementSpeed(float speed) { movementSpeed = speed; }
    void setMouseSensitivity(float sensitivity) { mouseSensitivity = sensitivity; }

private:
    // Calculates the front vector from camera's updated Euler angles
    void updateCameraVectors();
};
