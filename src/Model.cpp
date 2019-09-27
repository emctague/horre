// Created by Ethan McTague on 2019-09-22.

#include <string>
#include <fstream>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "Model.h"

Model::Model(ResourceSet *set, const std::string &path) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile("../res/model/" + path, aiProcess_Triangulate | aiProcess_FlipUVs |
                                                   aiProcess_OptimizeMeshes | aiProcess_GenNormals |
                                                   aiProcess_CalcTangentSpace | aiProcess_RemoveRedundantMaterials);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        throw std::runtime_error("Unable to open file: " + path + " (reason: " + importer.GetErrorString() + ")");

    processNode(set, scene, scene->mRootNode, glm::mat4{1});
    std::cout << std::endl;
}

Model::~Model() {
}

void Model::processNode(ResourceSet *set, const aiScene *scene, aiNode *node, glm::mat4 transform) {
    std::cout << "Node: " << node->mName.C_Str() << " (";

    aiMatrix4x4 &m = node->mTransformation;

    glm::mat4 nodeTransform = transform * glm::mat4{
            m.a1, m.b1, m.c1, m.d1,
            m.a2, m.b2, m.c2, m.d2,
            m.a3, m.b3, m.c3, m.d3,
            m.a4, m.b4, m.c4, m.d4
    };

    for (unsigned i = 0; i < node->mNumMeshes; i++) {
        std::cout << scene->mMeshes[node->mMeshes[i]]->mName.C_Str() << "( ";
        meshes.emplace_back(std::make_unique<Mesh>(set, scene, scene->mMeshes[node->mMeshes[i]], nodeTransform));
        std::cout << "), ";
    }

    std::cout << ")" << std::endl << "\t";

    for (unsigned i = 0; i < node->mNumChildren; i++) {
        processNode(set, scene, node->mChildren[i], nodeTransform);
    }
}
