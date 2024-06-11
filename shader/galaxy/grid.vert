#version 450

layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec2 a_texPos;

//----------------------------------------------------------------------------//

layout(location = 0) out vec2 o_texPos;

//----------------------------------------------------------------------------//

layout(set = 0, binding = 0) uniform Camera
{
    mat4 u_view;
    mat4 u_proj;
    mat4 u_viewProj;
};

layout(push_constant) uniform Params
{
    vec2 u_offset;
    mat4 u_model;
    int u_numCells;
    float u_thickness;
    float u_scroll; // in [1, 2]
};

//----------------------------------------------------------------------------//

void main()
{
    o_texPos = a_texPos;
    gl_Position = u_viewProj * u_model * vec4(a_pos, 1.0);
}