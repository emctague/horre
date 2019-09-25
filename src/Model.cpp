// Created by Ethan McTague on 2019-09-22.

#include <string>
#include <fstream>
#include <vector>
#include "Model.h"

Model::Model(const std::string &path) {
    std::vector<float> vertices;
    std::ifstream bin(path, std::ios::ate | std::ios::binary);

    if (!bin.is_open()) throw std::runtime_error("Unable to open file: " + path);

    // Read file contents as an array of floats
    int size = bin.tellg();
    bin.seekg(0, bin.beg);
    vertices.resize(size / sizeof(float));
    bin.read((char *) vertices.data(), size);
    bin.close();

    // Copy the model data into the GPU
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    vertCount = vertices.size() / 3;
}

Model::~Model() {
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}
