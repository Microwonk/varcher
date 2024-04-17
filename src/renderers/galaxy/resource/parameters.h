#pragma once

#include <glm/glm.hpp>

struct Camera {
    glm::mat4 u_view;
    glm::mat4 u_proj;
    glm::mat4 u_viewProj;
};

struct Params {
    glm::vec2 u_offset;
    int u_numCells;
    float u_thickness;
    float u_scroll; // in [1, 2]
};
