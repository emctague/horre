#pragma once

#include <glm/glm.hpp>
#include "Model.h"
#include "Shader.h"

class Entity {
public:
    Entity(std::shared_ptr<Model> model, std::shared_ptr<Shader> shader, glm::vec3 position);

    void render(glm::mat4 projection, glm::mat4 view);

    bool visible = true;
    glm::vec3 position;
    glm::vec3 size;
    std::shared_ptr<Model> model;
    std::shared_ptr<Shader> shader;

    void (*volume_trigger)(Entity *other);
};


