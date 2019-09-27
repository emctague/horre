#version 330 core

in vec3 fragPos;
in vec2 diffuseUV;
in vec2 alphaUV;

out vec4 color;

uniform sampler2D diffuse;
uniform sampler2D alpha;

vec3 lightPos = vec3(3.0, 3.0, 3.0);

void main() {
    float a = texture(alpha, alphaUV).r;
    if (a < 0.1) discard;
    color = vec4(texture(diffuse, diffuseUV).xyz * (10.0 / distance(fragPos, lightPos)), a);
}
