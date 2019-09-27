// Created by Ethan McTague on 2019-09-24.

#include "Entity.h"

#include <utility>

Entity::Entity(std::shared_ptr<Model> model, std::shared_ptr<Shader> shader, glm::vec3 position) :
        model(std::move(model)),
        shader(std::move(shader)),
        position(position),
        volume_trigger(nullptr) {
}

void Entity::render(glm::mat4 projection, glm::mat4 view, glm::vec3 viewPos) {
    shader->use();

    glm::mat4 modelMatrix{1.0f};
    modelMatrix = glm::scale(modelMatrix, size);

    // Note: these rotations are not necessarily correct and should be fixed.
    modelMatrix = glm::rotate(modelMatrix, roll, glm::vec3(1, 0, 0));
    modelMatrix = glm::rotate(modelMatrix, pitch, glm::vec3(0, 1, 0));
    modelMatrix = glm::rotate(modelMatrix, yaw, glm::vec3(0, 0, 1));

    modelMatrix = glm::translate(modelMatrix, position);

    shader->uniform("model", modelMatrix);
    shader->uniform("projection", projection);
    shader->uniform("view", view);
    shader->uniform("viewPos", viewPos);

    model->draw(shader.get());
}

Entity::~Entity() {
}
