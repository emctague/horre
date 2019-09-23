#include "Shader.h"
#include <utility>
#include <sstream>
#include <fstream>

Shader::Shader(std::string vertexPath, std::string fragmentPath) {
    auto vertexShader = loadShader(std::move(vertexPath), GL_VERTEX_SHADER);
    auto fragmentShader = loadShader(std::move(fragmentPath), GL_FRAGMENT_SHADER);

    program = glCreateProgram();

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    glUseProgram(program);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader() {
    glDeleteProgram(program);
}

unsigned Shader::loadShader(std::string path, unsigned type) {
    std::ifstream input(path);
    std::string source((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
    const auto cSource = source.c_str();
    const int length = source.length();

    auto shader = glCreateShader(type);
    glShaderSource(shader, 1, &cSource, &length);
    glCompileShader(shader);

    return shader;
}
