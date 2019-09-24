#pragma once

#include <glm/glm.hpp>
#include "Model.h"
#include "Shader.h"

class Entity {
public:
    Entity (Model model, glm::vec3 position);
    void render(Shader& shader, glm::mat4 projection, glm::mat4 view);

    glm::vec3 position;

private:
    Model model;

};


