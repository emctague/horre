#pragma once

#include "glinc.h"
#include "Texture.h"
#include "Shader.h"
#include <string>
#include <iostream>
#include <assimp/scene.h>
#include "Mesh.h"
#include <iostream>

class Mesh;

/// A loaded OpenGL model / mesh
class Model {
public:

    /// Load a 3D model from the specified file
    explicit Model(ResourceSet *set, const std::string &path);

    ~Model();

    /// Render this model (must be `use()`d first.)
    void draw(Shader *shader) {
        for (auto &mesh : meshes)
            mesh->draw(shader);
    }

private:
    std::vector<std::unique_ptr<Mesh>> meshes;

    void processNode(ResourceSet *set, const aiScene *scene, aiNode *node, glm::mat4 transform);
};

#include "ResourceSet.h"
