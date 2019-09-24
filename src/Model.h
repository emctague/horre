#pragma once
#include "glinc.h"
#include <string>

/// A loaded OpenGL model / mesh
class Model {
public:

    /// Load a 3D model from the specified file
    explicit Model (const std::string& path);

    /// Make this model active for rendering
    void use() { glBindVertexArray(vao); }

    /// Render this model (must be `use()`d first.)
    void draw() { glDrawArrays(GL_TRIANGLES, 0, vertCount); }

private:
    unsigned vao, vbo, vertCount;
};


