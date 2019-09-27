// Created by Ethan McTague on 2019-09-25.

#pragma once

#include "ResourceManager.h"
#include "Texture.h"
#include "Shader.h"
#include "Model.h"

/** Handles all resources of all types. */
class ResourceSet {
public:
    ResourceManager<Texture, std::string> textures;
    ResourceManager<Shader, std::string, std::string> shaders;
    ResourceManager<Model, ResourceSet *, std::string> models;

    ~ResourceSet();

    /** Clean up all resources, returning a count of all remaining loaded resources. */
    int cleanup();
};
