#include "camera_controller.h"

#include <GLFW/glfw3.h>
#include "engine/engine.h"

CameraController::CameraController(const std::shared_ptr<Engine>& engine, glm::vec3 position, float yaw, float pitch, float focalLength)
    : engine(engine), position(position), yaw(yaw), pitch(pitch), focalLength(focalLength)
{
    updateDirectionVectors();
    engine->inputs.cursorPosCallbacks.emplace_back([&](GLFWwindow* window, double xpos, double ypos) {
        mouseCallback(window, xpos, ypos);
    });
}

void CameraController::updateDirectionVectors()
{
    const glm::vec3 worldUp = glm::vec3(0.0f, -1.0f, 0.0f);

    normalDir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    normalDir.y = sin(glm::radians(pitch));
    normalDir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    normalDir = glm::normalize(normalDir);

    right = glm::normalize(glm::cross(normalDir, worldUp));
    up = glm::normalize(glm::cross(right, normalDir));

    direction = normalDir * focalLength;
    // fmt::println("x{}, y{}, z{}", direction.x, direction.y, direction.z);
}

void CameraController::update(float delta)
{
    const float cameraSpeed = 50.0f;

    if (glfwGetKey(engine->window, GLFW_KEY_W) == GLFW_PRESS)
        position += cameraSpeed * normalDir * delta;
    if (glfwGetKey(engine->window, GLFW_KEY_S) == GLFW_PRESS)
        position -= cameraSpeed * normalDir * delta;
    if (glfwGetKey(engine->window, GLFW_KEY_A) == GLFW_PRESS)
        position -= right * cameraSpeed * delta;
    if (glfwGetKey(engine->window, GLFW_KEY_D) == GLFW_PRESS)
        position += right * cameraSpeed * delta;

    updateDirectionVectors();
}

void CameraController::mouseCallback(GLFWwindow* window, double cursorX, double cursorY)
{
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        if (lastCursorX > 0 && lastCursorY > 0)
        {
            float offsetX = static_cast<float>(cursorX - lastCursorX);
            float offsetY = static_cast<float>(cursorY - lastCursorY);

            yaw -= offsetX;
            pitch = glm::clamp(glm::vec1(pitch - offsetY), -90.0f, 90.0f).x;
            updateDirectionVectors();
        }

        lastCursorX = cursorX;
        lastCursorY = cursorY;
    }
    else
    {
        lastCursorX = -1;
        lastCursorY = -1;
    }
}
