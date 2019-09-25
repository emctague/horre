#include "glinc.h"
#include "Shader.h"
#include "Model.h"
#include "Entity.h"
#include "Window.h"
#include <glm/glm.hpp>
#include <vector>
#include <iostream>


class App : public IWindowUser {
public:

    App() :
            window(glm::i64vec2{1280, 720}, "Title", this),
            projection() {
        window.setCursorEnabled(false);

        auto entShader = std::make_shared<Shader>("../test/test.vert", "../test/test.frag");
        auto entModel = std::make_shared<Model>("../test/test.bin");

        entities.emplace_back(entModel, entShader, glm::vec3(0, 0, 3));
        entities.emplace_back(entModel, entShader, glm::vec3(0, 0, 0));
        entities.emplace_back(entModel, entShader, glm::vec3(0, 0, 6));
        entities.emplace_back(entModel, entShader, glm::vec3(3, 0, 0));
        entities.emplace_back(entModel, entShader, glm::vec3(-3, 0, 0));

        cameraPos = &entities[0].position;
        entities[0].visible = false;
    }

    void run() {
        window.mainLoop();
    }

    void update(float deltaTime, glm::vec2 mouseDelta) override {

        // Handle mouse movement
        if (mouseDelta.x != 0 || mouseDelta.y != 0) {
            float sens = 0.05f;
            mouseDelta *= sens;
            cameraYaw += mouseDelta.x;
            cameraPitch += mouseDelta.y;

            if (cameraPitch > 89.0f) cameraPitch = 89.0f;
            if (cameraPitch < -89.0f) cameraPitch = -89.0f;
        }

    }

    void render() override {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::vec3 front = glm::normalize(glm::vec3{
                cos(glm::radians(cameraPitch)) * cos(glm::radians(cameraYaw)),
                sin(glm::radians(cameraPitch)),
                cos(glm::radians(cameraPitch)) * sin(glm::radians(cameraYaw))
        });
        glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0, 1, 0)));
        glm::vec3 up = glm::normalize(glm::cross(right, front));
        glm::mat4 view = glm::lookAt(*cameraPos, *cameraPos + front, up);

        for (auto &entity : entities) {
            if (entity.visible)
                entity.render(projection, view);
        }
    }

    void onResize(glm::i64vec2 size) override {
        projection = glm::perspective(
                45.0f,
                (float) size.x / (float) size.y,
                0.1f,
                100.0f
        );
    }

private:
    Window window; /**< Handle for window, graphics + input. */
    std::vector<Entity> entities; /**< All entities in the scene. */
    float cameraPitch = 0; /**< The pitch of the camera. */
    float cameraYaw = -90.0f;   /**< The yaw of the camera. */
    glm::mat4 projection; /**< Projection matrix used. */
    glm::vec3 *cameraPos;
};


int main() {
    App app;
    app.run();
    return 0;
}
