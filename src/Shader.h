#pragma once

#include <string>
#include "glinc.h"

/// OpenGL shader program instances
class Shader {
public:

    /// Create a shader program given the paths to its vertex and fragment shader source files
    Shader(const std::string& vertexPath, const std::string& fragmentPath);

    ~Shader();

    /// Make this shader program active
    void use () { glUseProgram(program); }

private:

    /// The OpenGL shader program instance being used
    unsigned program;

    /// Load and compile a shader - type is an OpenGL enum type
    static unsigned loadShader(const std::string& path, unsigned type);
};


