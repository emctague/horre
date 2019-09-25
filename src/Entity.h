#pragma once

#include <glm/glm.hpp>
#include "Model.h"
#include "Shader.h"
#include "Window.h"

class Entity {
public:
    Entity(std::shared_ptr<Model> model, std::shared_ptr<Shader> shader, glm::vec3 position);

    /** Render the entity onscreen with the given matrices. */
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

    bool visible = true; /**< Indicates if the entity should be rendered or not. */
    glm::vec3 position; /**< Stores the current position of the entity. */
    glm::vec3 size { 1.0 }; /**< The scale of the entity. (1, 1, 1) = regular size. */
    float yaw = 0.0f, pitch = 0.0f, roll = 0.0f; /**< The rotation of the entity. */
    std::shared_ptr<Model> model; /**< The model used when rendering this entity. */
    std::shared_ptr<Shader> shader; /**< The shader used when rendering this entity. */

    /** A function invoked when an entity overlaps this one's volume. */
    void (*volume_trigger)(Entity *other) = nullptr;

    /** A function invoked on each frame to update the entity's state. */
    void (*update)(Window *window, Entity *self, float deltaTime) = nullptr;
};


