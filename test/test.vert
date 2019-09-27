#version 330 core

layout (location=0) in vec3 pos;
layout (location=1) in vec3 tangent;
layout (location=2) in vec3 bitangent;
layout (location=3) in vec3 normal;
layout (location=4) in vec2 aDiffuseUV;
layout (location=5) in vec2 aAlphaUV;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 mesh;

out vec3 fragPos;
out vec2 diffuseUV;
out vec2 alphaUV;

void main() {
    fragPos = (mesh * model * vec4(pos, 1.0)).xyz;
    diffuseUV = aDiffuseUV;
    alphaUV = aAlphaUV;
    gl_Position = projection * view * model * mesh * vec4(pos, 1.0);
}
