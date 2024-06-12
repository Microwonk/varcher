#pragma once

#include <glm/vec3.hpp>
#include "engine/engine.h"

constexpr float CAMERA_FOV = 45.0f;
constexpr float CAMERA_MAX_DIST = 200.0f;
constexpr float CAMERA_MIN_TILT = 15.0f;
constexpr float CAMERA_MAX_TILT = 89.0f;
constexpr float CAMERA_MAX_POSITION = 7000.0f;

struct GameCamera {
    void update(float dt, const std::shared_ptr<Engine>& engine);
    void scroll(float amt);
    glm::vec3 up  = {0.0f, 1.0f, 0.0f};
    glm::vec3 center = {0.0f, 0.0f, 0.0f};
    glm::vec3 targetCenter = {0.0f, 0.0f, 0.0f};
    float dist = CAMERA_MAX_DIST;
    float targetDist = CAMERA_MAX_DIST;
    float angle = 45.0f;
    float targetAngle = 45.0f;
    float tilt = 45.0f;
    float targetTilt = 45.0f;
    glm::vec3 pos = {0,0,5};

    template<typename T>
    void decayTo(T& value, T target, float rate, float dt);
};