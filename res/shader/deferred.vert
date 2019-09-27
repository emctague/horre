#version 330 core

layout (location=0) in vec3 pos;
layout (location=4) in vec2 aDiffuseUV;

out vec2 screenUV;

void main() {
    gl_Position = vec4(pos, 1.0);
    screenUV = aDiffuseUV;
}
