#version 330 core

in vec3 fragPos;
in vec2 diffuseUV;
in vec2 alphaUV;
in vec3 normal;

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpecular;

uniform sampler2D diffuse;
uniform sampler2D alpha;

void main() {
    if (texture(alpha, alphaUV).r < 0.1) discard;
    gPosition = fragPos;
    gNormal = normalize(normal);
    gAlbedoSpecular.rgb = texture(diffuse, diffuseUV).rgb;
    gAlbedoSpecular.a = 0.1; /* TODO Stop hardcoding that */
}
