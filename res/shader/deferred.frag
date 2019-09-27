#version 330 core

in vec2 screenUV;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpecular;
uniform vec3 viewPos;

vec3 lightPos = vec3(20.0, 20.0, 20.0);
vec3 lightColor = vec3(0.8, 0.8, 1.0);
const float pi = 3.14159265;
const float shininess = 0.2;
const float energyConservation = (8.0 + shininess) / (8.0 * pi);
const float quadraticAttenuation = 0.032f;

out vec4 color;

void main()
{
    vec3 fragPos = texture(gPosition, screenUV).rgb;
    vec3 normal = texture(gNormal, screenUV).rgb;
    vec3 albedo = texture(gAlbedoSpecular, screenUV).rgb;
    float specular = texture(gAlbedoSpecular, screenUV).a;

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 lightDir = normalize(lightPos - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    float distance = length(lightPos - fragPos);
    float attenuation = 1.0 / (quadraticAttenuation * (distance * distance));

    float diffuseStrength = max(dot(normal, lightDir), 0.0);
    vec3 diffuseColor = lightColor * diffuseStrength * albedo * attenuation;

    float specularStrength = energyConservation * pow(max(dot(normal, halfwayDir), 0.0), shininess);
    vec3 specularColor = specularStrength * lightColor * specular * attenuation;

    vec3 allLights = diffuseColor + specularColor;

    color = vec4(allLights, 1.0);

    //color = vec4(1,1,1,1);
}