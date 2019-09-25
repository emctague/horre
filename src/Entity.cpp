// Created by Ethan McTague on 2019-09-24.

#include "Entity.h"

Entity::Entity(std::shared_ptr<Model> model, std::shared_ptr<Shader> shader, glm::vec3 position) :
    model(model),
    shader(shader),
    position(position),
    volume_trigger(nullptr) {
}

void Entity::render(glm::mat4 projection, glm::mat4 view) {
    shader->use();
    model->use();

    glm::mat4 modelMatrix(1.0);
    modelMatrix = glm::translate(modelMatrix, position);
    shader->uniform("model", modelMatrix);

    shader->uniform("projection", projection);
    shader->uniform("view", view);

    model->draw();
}
