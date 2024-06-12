#pragma once

#include <glm/glm.hpp>

struct ViewUniforms {
    glm::mat4 view;
    glm::mat4 proj;
    glm::vec3 pos;
};

struct Vertex {
    glm::vec3 pos;
    glm::vec2 color;
};
