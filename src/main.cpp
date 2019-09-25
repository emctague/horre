#include "glinc.h"
#include "Shader.h"
#include "Model.h"
#include "Entity.h"
#include "Window.h"
#include <glm/glm.hpp>
#include <vector>


class App : public IWindowUser {
public:

    App() :
        window(glm::i64vec2 { 1280, 720 }, "Title"),
        projection(glm::perspective(glm::radians(75.0f), 16.0f / 9.0f, 0.0f, 100.0f))

    {
        window.setCursorEnabled(false);

        auto entShader = std::make_shared<Shader>("../test/test.vert", "../test/test.frag");
        auto entModel = std::make_shared<Model>("../test/test.bin");

        entities.emplace_back(entModel, entShader, glm::vec3(0, 0, 3));
        entities.emplace_back(entModel, entShader, glm::vec3(0, 0, 0));

        cameraPos = &entities[0].position;
    }

    void run () {
        window.mainLoop(this);
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

        glm::vec3 front = glm::normalize(glm::vec3 {
                cos(glm::radians(cameraPitch)) * cos(glm::radians(cameraYaw)),
                sin(glm::radians(cameraPitch)),
                cos(glm::radians(cameraPitch)) * sin(glm::radians(cameraYaw))
        });
        glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0, 1, 0)));
        glm::vec3 up = glm::normalize(glm::cross(right, front));
        glm::mat4 view = glm::lookAt(*cameraPos, *cameraPos + front, up);

        for (auto& entity : entities) {
            entity.render(projection, view);
        }
    }

private:
    Window window;
    std::vector<Entity> entities;
    float cameraPitch, cameraYaw;
    glm::mat4 projection;
    glm::vec3 *cameraPos;
};


int main() {
    App app;
    app.run();
    return 0;
}
