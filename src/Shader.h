#pragma once

#include <string>
#include "glinc.h"

class Shader {
public:
    Shader(std::string vertexPath, std::string fragmentPath);
    ~Shader();

    void use () { glUseProgram(program); }

private:
    unsigned program;

    unsigned loadShader(std::string path, unsigned type);
};


