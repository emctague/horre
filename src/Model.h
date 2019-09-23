#pragma once

/// A loaded OpenGL model / mesh
class Model {
public:

    /// Load a 3D model from the specified file
    explicit Model (const std::string& path);

private:
    unsigned vao, vbo, vertCount;
};


