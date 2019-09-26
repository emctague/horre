#version 330 core

in vec3 fragPos;
out vec4 color;

vec3 lightPos = vec3(3.0, 3.0, 3.0);

void main() {
    color = vec4(vec3(1.0, 1.0, 1.0) * (2.0 / distance(fragPos, lightPos)), 1.0);
}
