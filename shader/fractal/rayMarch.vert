#version 450

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec2 inUV;

layout (location = 0) out vec2 outUV;


void main() {
    outUV = inUV;
    gl_Position = vec4(inPos.xyz, 1.0);
    // gl_Position = ubo_.projectionMatrix * ubo_.viewMatrix * ubo_.modelMatrix * vec4(inPos.xyz, 1.0);
}