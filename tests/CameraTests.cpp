#include <gtest/gtest.h>
#include "Camera.hpp"
#include <glm/glm.hpp>

class CameraTest : public ::testing::Test {
protected:
    Camera* camera;

    void SetUp() override {
        camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
    }

    void TearDown() override {
        delete camera;
    }
};

TEST_F(CameraTest, InitialValues) {
    EXPECT_EQ(camera->position.z, 3.0f);
    EXPECT_EQ(camera->yaw, -90.0f);
    EXPECT_EQ(camera->pitch, 0.0f);
    EXPECT_FLOAT_EQ(camera->movementSpeed, 2.5f);
    EXPECT_FLOAT_EQ(camera->mouseSensitivity, 0.1f);
}

TEST_F(CameraTest, MoveForward) {
    glm::vec3 initialPos = camera->position;
    camera->moveForward(1.0f);
    // Default front is (0, 0, -1), so moving forward should decrease Z
    EXPECT_LT(camera->position.z, initialPos.z);
}

TEST_F(CameraTest, MoveBackward) {
    glm::vec3 initialPos = camera->position;
    camera->moveBackward(1.0f);
    // Default front is (0, 0, -1), so moving backward should increase Z
    EXPECT_GT(camera->position.z, initialPos.z);
}

TEST_F(CameraTest, MoveLeft) {
    glm::vec3 initialPos = camera->position;
    camera->moveLeft(1.0f);
    // Default right is (1, 0, 0), so moving left should decrease X
    EXPECT_LT(camera->position.x, initialPos.x);
}

TEST_F(CameraTest, MoveRight) {
    glm::vec3 initialPos = camera->position;
    camera->moveRight(1.0f);
    // Default right is (1, 0, 0), so moving right should increase X
    EXPECT_GT(camera->position.x, initialPos.x);
}

TEST_F(CameraTest, MoveUp) {
    glm::vec3 initialPos = camera->position;
    camera->moveUp(1.0f);
    // Default up is (0, 1, 0), so moving up should increase Y
    EXPECT_GT(camera->position.y, initialPos.y);
}

TEST_F(CameraTest, MoveDown) {
    glm::vec3 initialPos = camera->position;
    camera->moveDown(1.0f);
    // Default up is (0, 1, 0), so moving down should decrease Y
    EXPECT_LT(camera->position.y, initialPos.y);
}

TEST_F(CameraTest, ViewMatrix) {
    glm::mat4 view = camera->getViewMatrix();
    // Check if it's a valid matrix (not all zeros)
    EXPECT_NE(view[0][0], 0.0f); 
}
