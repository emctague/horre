#include "Shader.h"
#include <sstream>
#include <fstream>
#include <iostream>

Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath) {
    // Load and compile individual shaders
    auto vertexShader = loadShader(vertexPath, GL_VERTEX_SHADER);
    auto fragmentShader = loadShader(fragmentPath, GL_FRAGMENT_SHADER);

    program = glCreateProgram();

    // Link shaders together to form a shader program
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    glUseProgram(program);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Freak out if shader linking failed
    int status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (!status) {
        char log[512];
        glGetProgramInfoLog(program, 512, nullptr, log);
        glDeleteProgram(program);
        throw std::runtime_error("Compile error in shader program: " + std::string(log));
    }

}

Shader::~Shader() {
    glDeleteProgram(program);
}

unsigned Shader::loadShader(const std::string &path, unsigned type) {
    std::ifstream input("../res/shader/" + path);

    if (!input.is_open()) throw std::runtime_error("Unable to open file: " + path);

    // Load shader file source code
    std::string source((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
    const auto cSource = source.c_str();
    const int length = source.length();

    // Create and compile the shader
    auto shader = glCreateShader(type);
    glShaderSource(shader, 1, &cSource, &length);
    glCompileShader(shader);

    // Freak out if compilation failed
    int status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (!status) {
        char log[512];
        glGetShaderInfoLog(shader, 512, nullptr, log);
        throw std::runtime_error("Compile error in " + path + ": " + std::string(log));
    }

    return shader;
}
