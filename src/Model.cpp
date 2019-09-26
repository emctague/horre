// Created by Ethan McTague on 2019-09-22.

#include <string>
#include <fstream>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "Model.h"

Model::Model(const std::string &path) {
    std::vector<float> vertices;
    std::vector<unsigned> indices;

    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs |
                                                   aiProcess_OptimizeMeshes | aiProcess_CalcTangentSpace |
                                                   aiProcess_RemoveRedundantMaterials |
                                                   aiProcess_PreTransformVertices);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        throw std::runtime_error("Unable to open file: " + path + " (reason: " + importer.GetErrorString() + ")");

    processNode(scene, scene->mRootNode, vertices, indices);

    std::cout << indices.size() << std::endl;

    // Copy the model data into the GPU
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), indices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    indiceCount = indices.size();
}

Model::~Model() {
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}

void
Model::processNode(const aiScene *scene, aiNode *node, std::vector<float> &vertices, std::vector<unsigned> &indices) {
    for (unsigned i = 0; i < node->mNumMeshes; i++) {
        unsigned baseIndex = indices.empty() ? 0 : indices[indices.size() - 1] + 1;

        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        for (unsigned j = 0; j < mesh->mNumVertices; j++) {
            vertices.push_back(mesh->mVertices[j].x);
            vertices.push_back(mesh->mVertices[j].y);
            vertices.push_back(mesh->mVertices[j].z);
        }

        for (unsigned j = 0; j < mesh->mNumFaces; j++) {
            for (unsigned k = 0; k < mesh->mFaces[j].mNumIndices; k++) {
                indices.push_back(mesh->mFaces[j].mIndices[k] + baseIndex);
            }
        }
    }

    for (unsigned i = 0; i < node->mNumChildren; i++) {
        processNode(scene, node->mChildren[i], vertices, indices);
    }
}
