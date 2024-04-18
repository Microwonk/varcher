#pragma once

#include <glm/glm.hpp>

struct Camera {
    glm::mat4 u_view;
    glm::mat4 u_proj;
    glm::mat4 u_viewProj;
};
