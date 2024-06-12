#pragma once

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

struct GLFWwindow;
class Engine;

class CameraController
{
public:
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 right;
    glm::vec3 up;

private:
    float yaw;
    float pitch;
    glm::vec3 normalDir;

    float focalLength;

    double lastCursorX = -1;
    double lastCursorY = -1;

    std::shared_ptr<Engine> engine;

public:
    CameraController() = default;
    CameraController(const std::shared_ptr<Engine>& engine, glm::vec3 position, float yaw, float pitch, float focalLength);

    void update(float delta);
    void mouseCallback(GLFWwindow* window, double xpos, double ypos);

    [[nodiscard]] glm::mat4 getViewMatrix() const {
        // Assuming the camera's target direction is calculated elsewhere and stored in 'direction'
        return glm::lookAt(position, position + direction, up);
    }

    [[nodiscard]] glm::mat4 getProjectionMatrix(float fov, float aspectRatio, float nearPlane, float farPlane) const {
        return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
    }

private:
    void updateDirectionVectors();
};
