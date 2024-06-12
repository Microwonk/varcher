#pragma once

#include <glm/glm.hpp>
#include "camera.h"

struct ViewUniforms {
    glm::mat4 view;
    glm::mat4 proj;
    glm::vec3 pos;
};

struct GridParams {
    GameCamera cam;
    float fov = CAMERA_FOV;
};
