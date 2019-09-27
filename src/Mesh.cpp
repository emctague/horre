// Created by Ethan McTague on 2019-09-26.

#include "Mesh.h"
#include <iostream>
#include "ResourceSet.h"


/** Base case of OpenGL attrib pointer template tool. */
template<typename P, typename T>
inline void attribPointers(int n = 0, size_t s = 0) {
    glEnableVertexAttribArray(n);
    glVertexAttribPointer(n, sizeof(T) / sizeof(float), GL_FLOAT, GL_FALSE, sizeof(P), (void *) s);
}

/** Applies all OpenGL attrib pointers given as type arguments (first argument is vertex class). */
template<typename P, typename T, typename S, typename ...Args>
inline void attribPointers(int n = 0, size_t s = 0) {
    attribPointers<P, T>(n, s);
    attribPointers<P, S, Args...>(n + 1, s + sizeof(T));
}


Mesh::Mesh(ResourceSet *set, const aiScene *scene, aiMesh *mesh, glm::mat4 transform) :
        diffuse(nullptr), alpha(nullptr), transform(transform) {
    std::vector<Vertex> vertices;
    std::vector<unsigned> indices;


    for (unsigned j = 0; j < mesh->mNumVertices; j++) {
        glm::vec2 diffuseUV = {mesh->mTextureCoords[0] ? mesh->mTextureCoords[0][j].x : 0,
                               mesh->mTextureCoords[0] ? mesh->mTextureCoords[0][j].y : 0};

        vertices.push_back({
                                   .position = {mesh->mVertices[j].x, mesh->mVertices[j].y, mesh->mVertices[j].z},
                                   .tangent = {mesh->mTangents[j].x, mesh->mTangents[j].y, mesh->mTangents[j].z},
                                   .bitangent = {mesh->mBitangents[j].x, mesh->mBitangents[j].y,
                                                 mesh->mBitangents[j].z},
                                   .normal = {mesh->mNormals[j].x, mesh->mNormals[j].y, mesh->mNormals[j].z},
                                   .diffuseUV = diffuseUV,
                                   .alphaUV = {mesh->mTextureCoords[1] ? mesh->mTextureCoords[1][j].x : diffuseUV.x,
                                               mesh->mTextureCoords[1] ? mesh->mTextureCoords[1][j].y : diffuseUV.y}
                           });
    }

    for (unsigned j = 0; j < mesh->mNumFaces; j++) {
        for (unsigned k = 0; k < mesh->mFaces[j].mNumIndices; k++) {
            indices.push_back(mesh->mFaces[j].mIndices[k]);
        }
    }

    // Create OpenGL objects to hold model data
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), indices.data(), GL_STATIC_DRAW);

    auto sizes = {3, 3, 3, 3, 2, 2};

    attribPointers<Vertex, glm::vec3, glm::vec3, glm::vec3, glm::vec3, glm::vec2, glm::vec2>();

    indiceCount = indices.size();

    if (mesh->mMaterialIndex >= 0) {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        aiString path;

        if (material->GetTextureCount(aiTextureType_DIFFUSE)) {
            material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
            std::cout << "DIFFUSE = " << path.C_Str() << ", ";
            diffuse = set->textures.getResource(std::string(path.C_Str()));
        } else {
            diffuse = set->textures.getResource("../test/default_diffuse.png");
        }

        if (material->GetTextureCount(aiTextureType_OPACITY)) {
            material->GetTexture(aiTextureType_OPACITY, 0, &path);
            std::cout << "ALPHA =  " << path.C_Str() << ", ";
            alpha = set->textures.getResource(std::string(path.C_Str()));
        } else {
            alpha = set->textures.getResource("../test/default_alpha.png");
        }
    }
}

void Mesh::draw(Shader *shader) {
    shader->uniform("diffuse", 0);
    shader->uniform("alpha", 1);
    shader->uniform("mesh", transform);

    diffuse->use(0);
    alpha->use(1);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indiceCount, GL_UNSIGNED_INT, nullptr);
}

Mesh::~Mesh() {
    glDeleteBuffers(1, &vao);
    glDeleteVertexArrays(1, &vbo);
    glDeleteVertexArrays(1, &ebo);
}

