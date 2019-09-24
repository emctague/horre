#include "glinc.h"
#include "Shader.h"
#include "Model.h"
#include <glm/glm.hpp>

GLFWwindow *init();

int main() {
    auto window = init();

    Model mdl("../test/test.bin");
    Shader garbage("../test/test.vert", "../test/test.frag");

    glm::mat4 projection = glm::perspective(90.0f, 16.0f / 9.0f, 0.0f, 100.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, -1.0));

    garbage.use();
    garbage.uniform("projection", projection);
    garbage.uniform("view", view);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mdl.use();
        garbage.use();
        mdl.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

GLFWwindow *init() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

    auto window = glfwCreateWindow(1280, 720, "Spoopy Game", nullptr, nullptr);

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    return window;
}