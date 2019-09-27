#pragma once

class ResourceSet;

#include <assimp/scene.h>
#include <vector>
#include <memory>
#include "Texture.h"
#include "Shader.h"

class Mesh {
public:
    Mesh(ResourceSet *set, const aiScene *scene, aiMesh *mesh, glm::mat4 transform);

    ~Mesh();

    void draw(Shader *shader);

private:
    std::shared_ptr<Texture> diffuse, alpha;
    glm::mat4 transform;

    unsigned vao = 0, vbo = 0, ebo = 0, indiceCount = 0;
};

struct Vertex {
    glm::vec3 position;
    glm::vec3 tangent;
    glm::vec3 bitangent;
    glm::vec3 normal;
    glm::vec2 diffuseUV;
    glm::vec2 alphaUV;
};