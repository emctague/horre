#pragma once

#include <string>
#include "glinc.h"
#include <glm/gtc/type_ptr.hpp>

/// OpenGL shader program instances
class Shader {
public:

    /// Create a shader program given the paths to its vertex and fragment shader source files
    Shader(const std::string &vertexPath, const std::string &fragmentPath);

    ~Shader();

    /// Make this shader program active
    void use() { glUseProgram(program); }

    void uniform(const std::string &name, glm::mat4 value) {
        glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
    }

    void uniform(const std::string &name, int value) {
        glUniform1i(glGetUniformLocation(program, name.c_str()), value);
    }

private:

    /// The OpenGL shader program instance being used
    unsigned program;

    /// Load and compile a shader - type is an OpenGL enum type
    static unsigned loadShader(const std::string &path, unsigned type);
};


