#version 330 core

in vec3 fragPos;
in vec2 diffuseUV;
in vec2 alphaUV;
in vec3 normal;

out vec4 color;

uniform sampler2D diffuse;
uniform sampler2D alpha;
uniform vec3 viewPos;

vec3 lightPos = vec3(20.0, 20.0, 20.0);
vec3 lightColor = vec3(0.8, 0.8, 1.0);
const float pi = 3.14159265;
const float shininess = 0.2;
const float energyConservation = (8.0 + shininess) / (8.0 * pi);
const float quadraticAttenuation = 0.032f;

void main() {
    float a = texture(alpha, alphaUV).r;
    if (a < 0.1) discard;

    float distance = length(lightPos - fragPos);
    float attenuation = 1.0 / (quadraticAttenuation * (distance * distance));
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    float diffuseStrength = max(dot(norm, lightDir), 0.0);
    vec3 diffuseColor = lightColor * diffuseStrength * attenuation;

    float specularStrength = energyConservation * pow(max(dot(normal, halfwayDir), 0.0), shininess);
    vec3 specularColor = specularStrength * lightColor * attenuation;

    vec3 allLights = (diffuseColor + specularColor) * texture(diffuse, diffuseUV).rgb;

    color = vec4(allLights, a);
}
