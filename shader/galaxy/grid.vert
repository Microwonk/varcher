#version 450

layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec2 a_texPos;

layout(location = 0) out vec2 o_texPos;

layout(set = 0, binding = 0) uniform ViewUniforms {
    mat4 view;
    mat4 proj;
    vec3 pos;
} view;

const vec3 gridPlane[6] = vec3[](
vec3(1, 1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
vec3(-1, -1, 0), vec3(1, 1, 0), vec3(1, -1, 0)
);

void main() {
    o_texPos = a_texPos;
    gl_Position = view.proj * view.view * vec4(gridPlane[gl_VertexIndex], 1.0);
}