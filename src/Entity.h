#pragma once

#include <glm/glm.hpp>
#include "Model.h"
#include "Shader.h"
#include "Window.h"

class Entity {
public:
    Entity(std::shared_ptr<Model> model, std::shared_ptr<Shader> shader, glm::vec3 position);

    void render(glm::mat4 projection, glm::mat4 view);

    /** Calculate the forward-facing direction for this entity. */
    glm::vec3 getFront() {
        return glm::normalize(glm::vec3{
                cos(glm::radians(pitch)) * cos(glm::radians(yaw)),
                sin(glm::radians(pitch)),
                cos(glm::radians(pitch)) * sin(glm::radians(yaw))
        });
    }

    /** Calculate the right-facing direction for this entity. */
    glm::vec3 getRight() {
        return glm::normalize(glm::cross(getFront(), glm::vec3(0, 1, 0)));
    }

    bool visible = true;
    glm::vec3 position;
    glm::vec3 size { 1.0 };
    float yaw = 0.0f, pitch = 0.0f;
    std::shared_ptr<Model> model;
    std::shared_ptr<Shader> shader;

    void (*volume_trigger)(Entity *other) = nullptr;
    void (*update)(Window *window, Entity *self, float deltaTime) = nullptr;
};


