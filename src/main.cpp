#include "glinc.h"
#include "Shader.h"
#include "Model.h"
#include <glm/glm.hpp>

struct CameraState {
    float pitch, yaw, last_x, last_y;
};

GLFWwindow *init();
void on_mouse(GLFWwindow *window, double xpos, double ypos);

int main() {

    auto window = init();
    CameraState cameraState { 0.0, 0.0, 640, 360 };
    glm::vec3 cameraPos { 0.0f, 0.0f, 3.0f };

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetWindowUserPointer(window, &cameraState);
    glfwSetCursorPosCallback(window, on_mouse);

    Model mdl("../test/test.bin");
    Shader garbage("../test/test.vert", "../test/test.frag");

    glm::mat4 projection = glm::perspective(glm::radians(75.0f), 16.0f / 9.0f, 0.0f, 100.0f);

    garbage.use();
    garbage.uniform("projection", projection);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::vec3 front = glm::normalize(glm::vec3 {
                cos(glm::radians(cameraState.pitch)) * cos(glm::radians(cameraState.yaw)),
                sin(glm::radians(cameraState.pitch)),
                cos(glm::radians(cameraState.pitch)) * sin(glm::radians(cameraState.yaw))
        });
        glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0, 1, 0)));
        glm::vec3 up = glm::normalize(glm::cross(right, front));
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + front, up);

        mdl.use();
        garbage.use();
        garbage.uniform("view", view);
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

void on_mouse(GLFWwindow *window, double xpos, double ypos) {
    auto state = (CameraState*)glfwGetWindowUserPointer(window);
    float offset_x = xpos - state->last_x;
    float offset_y = state->last_y - ypos;
    state->last_x = xpos;
    state->last_y = ypos;

    float sens = 0.05f;
    offset_x *= sens;
    offset_y *= sens;

    state->yaw += offset_x;
    state->pitch += offset_y;

    if (state->pitch > 89.0f) state->pitch = 89.0f;
    if (state->pitch < -89.0f) state->pitch = -89.0f;
}
