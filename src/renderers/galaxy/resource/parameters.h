#pragma once

#include <glm/glm.hpp>
#include "camera.h"

struct Camera {
    glm::mat4 u_view;
    glm::mat4 u_proj;
    glm::mat4 u_viewProj;
};

struct GridParams {
    GameCamera cam;
    float fov = CAMERA_FOV;
};
