#include "glinc.h"
#include "Shader.h"
#include "Entity.h"
#include "Window.h"
#include "ResourceSet.h"
#include "Framebuffer.h"
#include <glm/glm.hpp>
#include <vector>
#include <fstream>
#include <sstream>
#include <unordered_map>

class App : public IWindowUser {
public:

    using UpT = void(*)(Window*, Entity*, float);
    
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
                
        updaters.insert({"movable", [](Window *pWindow, Entity *self, float deltaTime) {
            auto front = self->getFront();
            auto right = self->getRight();
            if (pWindow->keyIsDown(GLFW_KEY_W)) self->position += front * deltaTime * 2.0f;
            if (pWindow->keyIsDown(GLFW_KEY_S)) self->position -= front * deltaTime * 2.0f;
            if (pWindow->keyIsDown(GLFW_KEY_D)) self->position += right * deltaTime * 2.0f;
            if (pWindow->keyIsDown(GLFW_KEY_A)) self->position -= right * deltaTime * 2.0f;
        }});
                
        updaters.insert({"bouncy", [](Window *pWindow, Entity *self, float deltaTime) {
            self->size += glm::vec3(sin(glfwGetTime() * 2.0f)) * 0.00001f;
        }});

        loadMap("main.map");
       
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
            if (entity->update) {
                entity->update(&window, entity.get(), deltaTime);
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
            if (entity->visible)
                entity->render(projection, view, camera->position);
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

    void loadMap(const std::string& path) {
        entities.clear();
        camera = nullptr;

        std::ifstream in(GlobalConfig_MapPath + path);
        if (!in.is_open()) throw std::runtime_error("Failed to open map file: " + path);

        std::string line;
        while (std::getline(in, line)) {
            std::stringstream tokenizer(line);
            std::string token;
            std::vector<std::string> tokens;

            while (getline(tokenizer, token, ' ')) tokens.push_back(token);
            
            bool isCamera = false;
            bool isVisible = true;
            glm::vec3 position { 0.0f, 0.0f, 0.0f };
            glm::vec3 scale { 1.0f, 1.0f, 1.0f };
            std::string vertexShader = "test.vert";
            std::string fragmentShader = "test.frag";
            std::string model = "screen.dae";
            float pitch = 0, yaw = 0;
            UpT updater = nullptr;

            for (auto& token : tokens) {
                std::string key = token.substr(0, token.find('='));
                std::string value = token.substr(token.find('=') + 1);
                
                /* TODO replace this with some nice parsing from a data structure */
                if (key == "m") model = value;
                else if (key == "v") vertexShader = value;
                else if (key == "f") fragmentShader = value;
                else if (key == "x") position.x = std::stof(value);
                else if (key == "y") position.y = std::stof(value);
                else if (key == "z") position.z = std::stof(value);
                else if (key == "s") scale *= std::stof(value);
                else if (key == "sx") scale.x *= std::stof(value);
                else if (key == "sy") scale.y *= std::stof(value);
                else if (key == "sz") scale.z *= std::stof(value);
                else if (key == "camera") isCamera = value == "true";
                else if (key == "visible") isVisible = value == "true";
                else if (key == "ry") yaw = std::stof(value);
                else if (key == "rp") pitch = std::stof(value);
                else if (key == "updater") updater = updaters[value];
                else std::cout << "Unknown key: " << key << std::endl;
            }
            
            entities.emplace_back(std::make_unique<Entity>(
                   resources.models.getResource(&resources, std::string(model)),
                   resources.shaders.getResource(std::string(vertexShader), std::string(fragmentShader)),
                       position));
            
            entities.back()->visible = isVisible;
            entities.back()->size = scale;
            entities.back()->yaw = yaw;
            entities.back()->pitch = pitch;
            entities.back()->update = updater;
            if (isCamera) camera = entities.back().get();
        }

    }

private:
    Window window; /**< Handle for window, graphics + input. */
    Framebuffer mainBuffer; /**< Main framebuffer handle */
    Framebuffer gBuffer; /**< GBuffer (deferred rendering) target */
    ResourceSet resources; /**< Keeps track of all resources. */
    std::shared_ptr<Shader> deferredShader; /**< The shader used for deferred rendering. */
    std::shared_ptr<Model> deferredModel; /**< The model used for deferred rendering. */
    std::vector<std::unique_ptr<Entity>> entities; /**< All entities in the scene. */
    std::unordered_map<std::string, UpT> updaters; /**<All registered update functions for entities.*/
    Entity *camera = nullptr; /** The entity that acts as a camera object. */
    glm::mat4 projection; /**< Projection matrix used. */
};


int main() {
    App app;
    app.run();
    return 0;
}
