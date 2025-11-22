#include "Camera.hpp"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : position(position)
    , worldUp(up)
    , yaw(yaw)
    , pitch(pitch)
    , movementSpeed(2.5f)
    , mouseSensitivity(0.1f)
    , zoom(45.0f)
    , front(glm::vec3(0.0f, 0.0f, -1.0f))
{
    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(position, position + front, up);
}

void Camera::moveForward(float deltaTime)
{
    position += front * movementSpeed * deltaTime;
}

void Camera::moveBackward(float deltaTime)
{
    position -= front * movementSpeed * deltaTime;
}

void Camera::moveLeft(float deltaTime)
{
    position -= right * movementSpeed * deltaTime;
}

void Camera::moveRight(float deltaTime)
{
    position += right * movementSpeed * deltaTime;
}

void Camera::moveUp(float deltaTime)
{
    position += worldUp * movementSpeed * deltaTime;
}

void Camera::moveDown(float deltaTime)
{
    position -= worldUp * movementSpeed * deltaTime;
}

void Camera::processMouse(float xoffset, float yoffset, bool constrainPitch)
{
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // Constrain pitch to prevent screen flip
    if (constrainPitch)
    {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }

    updateCameraVectors();
}

void Camera::updateCameraVectors()
{
    // Calculate the new front vector from Euler angles
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);

    // Recalculate the right and up vectors
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}