#include "camera.h"

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

void GameCamera::update(float dt, const std::shared_ptr<Engine>& engine) {
    float camSpeed = 1.0f * dt * dist;
    float angleSpeed = 45.0f * dt;
    float tiltSpeed = 30.0f * dt;

    auto window = engine->window;

    glm::vec4 forward4 = glm::rotate(glm::mat4(1.0f), angle, up) * glm::vec4(0.0f, 0.0f, -1.0f, 1.0f);
    glm::vec4 side4 = glm::rotate(glm::mat4(1.0f), angle, up) * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

    glm::vec3 forward = {forward4.x, forward4.y, forward4.z};
    glm::vec3 side = {side4.x, side4.y, side4.z};

    glm::vec3 camVel(0.0f, 0.0f, 0.0f);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camVel = camVel + forward;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camVel = camVel - forward;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camVel = camVel + side;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camVel = camVel - side;

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        targetAngle -= angleSpeed;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        targetAngle += angleSpeed;

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        targetTilt = std::min(targetTilt + tiltSpeed, CAMERA_MAX_TILT);
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        targetTilt = std::max(targetTilt - tiltSpeed, CAMERA_MIN_TILT);

    targetCenter = targetCenter + camSpeed * glm::normalize(camVel);
    if (glm::length(targetCenter) > CAMERA_MAX_POSITION)
        targetCenter = glm::normalize(targetCenter) * CAMERA_MAX_POSITION;

    decayTo(center, targetCenter, 0.985f, dt);
    decayTo(dist, targetDist, 0.99f, dt);
    decayTo(angle, targetAngle, 0.99f, dt);
    decayTo(tilt, targetTilt, 0.99f, dt);

    glm::vec4 toPos = glm::rotate(glm::mat4(1.0f), -tilt, side) * glm::vec4(forward, 1.0f);
    pos = center - dist * glm::normalize(glm::vec3(toPos.x, toPos.y, toPos.z));
}

void GameCamera::scroll(float amt) {
    targetDist -= 0.1f * targetDist * amt;
    targetDist = std::round(targetDist * 100.0f) / 100.0f;

    // clamp
    if (targetDist > CAMERA_MAX_DIST) {
        targetDist = CAMERA_MAX_DIST;
    }
}

template<typename T>
void GameCamera::decayTo(T& value, T target, float rate, float dt) {
    value = value + (target - value) * (1.0f - std::pow(rate, 1000.0f * dt));
}