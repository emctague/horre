// Created by Ethan McTague on 2019-09-24.

#include "Entity.h"

Entity::Entity(Model model, glm::vec3 position) : model(model), position(position) {
}

void Entity::render(Shader &shader, glm::mat4 projection, glm::mat4 view) {
    shader.use();
    model.use();

    glm::mat4 modelMatrix(1.0);
    modelMatrix = glm::translate(modelMatrix, position);
    shader.uniform("model", modelMatrix);

    shader.uniform("projection", projection);
    shader.uniform("view", view);

    model.draw();
}
