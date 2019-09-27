#include "glinc.h"
#include "Shader.h"
#include "Entity.h"
#include "Window.h"
#include "ResourceSet.h"
#include "Framebuffer.h"
#include <glm/glm.hpp>
#include <vector>

class App : public IWindowUser {
public:

    App() :
            window(glm::i64vec2{1280, 720}, "Title", this),
            gBuffer(&window, {
                    {"gPosition",       GL_RGB16F, GL_RGB,  GL_FLOAT}, /* Position */
                    {"gNormal",         GL_RGB16F, GL_RGB,  GL_FLOAT}, /* Normal */
                    {"gAlbedoSpecular", GL_RGBA,   GL_RGBA, GL_UNSIGNED_BYTE} /* Color + Specular Color */
            }),
            projection() {
        window.setCursorEnabled(false);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glEnable(GL_FRAMEBUFFER_SRGB);
        //glEnable(GL_BLEND);
        //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        deferredModel = resources.models.getResource(&resources, "screen.dae");
        deferredShader = resources.shaders.getResource("deferred.vert", "deferred.frag");

        auto entShader = resources.shaders.getResource("test.vert", "test.frag");
        auto entModel = resources.models.getResource(&resources, "Tombstones_simple.dae");
        auto treeModel = resources.models.getResource(&resources, "tree.obj");

        entities.emplace_back(entModel, entShader, glm::vec3(-2, 1.5, 2));
        entities.emplace_back(entModel, entShader, glm::vec3(0, 0, 0));
        entities.emplace_back(entModel, entShader, glm::vec3(20, 0, 0));
        entities.emplace_back(treeModel, entShader, glm::vec3(0, 0, 0));
        entities.emplace_back(entModel, entShader, glm::vec3(40, 0, 0));
        entities.emplace_back(entModel, entShader, glm::vec3(60, 0, 0));

        camera = &entities[0];
        entities[0].visible = false;
        entities[0].yaw = -45.0f;
        entities[0].pitch = -5.0f;
        entities[0].update = [](Window *pWindow, Entity *self, float deltaTime) {
            auto front = self->getFront();
            auto right = self->getRight();
            if (pWindow->keyIsDown(GLFW_KEY_W)) self->position += front * deltaTime * 2.0f;
            if (pWindow->keyIsDown(GLFW_KEY_S)) self->position -= front * deltaTime * 2.0f;
            if (pWindow->keyIsDown(GLFW_KEY_D)) self->position += right * deltaTime * 2.0f;
            if (pWindow->keyIsDown(GLFW_KEY_A)) self->position -= right * deltaTime * 2.0f;
        };

        entities[1].size *= 0.02;
        entities[2].size *= 0.02;
        entities[3].size *= 0.1;
        entities[4].size *= 0.02;
        entities[5].size *= 0.02;
        entities[3].update = [](Window *pWindow, Entity *self, float deltaTime) {
            self->size += glm::vec3(sin(glfwGetTime() * 2.0f)) * 0.00001f;
            // self->size = glm::vec3(abs(sin(deltaTime / 10.0f)) * 3.0f);
            //self->roll += deltaTime * 0.2f;
            //self->pitch += deltaTime * 0.5f;
        };
    }

    void run() {
        window.mainLoop();
    }

    ~App() {
        deferredShader.reset();
        deferredModel.reset();

        // Unload all loaded entities
        entities.clear();
    }

    void update(float deltaTime, glm::vec2 mouseDelta) override {

        // Handle mouse movement
        if (mouseDelta.x != 0 || mouseDelta.y != 0) {
            float sens = 0.05f;
            mouseDelta *= sens;
            camera->yaw += mouseDelta.x;
            camera->pitch += mouseDelta.y;

            if (camera->pitch > 89.0f) camera->pitch = 89.0f;
            if (camera->pitch < -89.0f) camera->pitch = -89.0f;
        }

        for (auto &entity : entities) {
            if (entity.update) {
                entity.update(&window, &entity, deltaTime);
            }
        }

    }

    void render() override {
        gBuffer.bind();

        glClear((unsigned) GL_COLOR_BUFFER_BIT | (unsigned) GL_DEPTH_BUFFER_BIT);

        glm::vec3 front = camera->getFront();
        glm::vec3 up = glm::normalize(glm::cross(camera->getRight(), front));
        glm::mat4 view = glm::lookAt(camera->position, camera->position + front, up);

        for (auto &entity : entities) {
            if (entity.visible)
                entity.render(projection, view, camera->position);
        }

        //gBuffer.blitDepthTo(mainBuffer, 1280, 720);
        mainBuffer.bind();

        glDisable(GL_DEPTH_TEST);
        deferredShader->use();
        deferredShader->uniform("viewPos", camera->position);
        gBuffer.asTextures(deferredShader.get());
        deferredModel->draw(deferredShader.get(), true);
        glEnable(GL_DEPTH_TEST);
    }

    void onResize(glm::i64vec2 size) override {
        projection = glm::perspective(
                45.0f,
                (float) size.x / (float) size.y,
                0.1f,
                100.0f
        );
        gBuffer.updateSize(&window);
    }

private:
    Window window; /**< Handle for window, graphics + input. */
    Framebuffer mainBuffer; /**< Main framebuffer handle */
    Framebuffer gBuffer; /**< GBuffer (deferred rendering) target */
    ResourceSet resources; /**< Keeps track of all resources. */
    std::shared_ptr<Shader> deferredShader; /**< The shader used for deferred rendering. */
    std::shared_ptr<Model> deferredModel; /**< The model used for deferred rendering. */
    std::vector<Entity> entities; /**< All entities in the scene. */
    Entity *camera = nullptr; /** The entity that acts as a camera object. */
    glm::mat4 projection; /**< Projection matrix used. */
};


int main() {
    App app;
    app.run();
    return 0;
}
