#pragma once

#include <string>
#include "glinc.h"

class Shader {
public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();

    void use () { glUseProgram(program); }

private:
    unsigned program;

    static unsigned loadShader(const std::string& path, unsigned type);
};


