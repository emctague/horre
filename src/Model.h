#pragma once

#include "glinc.h"
#include <string>
#include <iostream>
#include <assimp/scene.h>

/// A loaded OpenGL model / mesh
class Model {
public:

    /// Load a 3D model from the specified file
    explicit Model(const std::string &path);

    ~Model();

    /// Render this model (must be `use()`d first.)
    void draw() {
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, indiceCount, GL_UNSIGNED_INT, nullptr);
    }

private:

    unsigned vao = 0, vbo = 0, ebo = 0, indiceCount = 0;

    /** Recurse through assimp nodes and add to the list of vertices and indices. */
    static void
    processNode(const aiScene *scene, aiNode *node, std::vector<float> &vertices, std::vector<unsigned> &indices);
};


