// Created by Ethan McTague on 2019-09-25.

#pragma once

#include "ResourceManager.h"
#include "Model.h"
#include "Shader.h"

/** Handles all resources of all types. */
class ResourceSet {
public:
    ResourceManager<Shader, std::string, std::string> shaders;
    ResourceManager<Model, std::string> models;
};


