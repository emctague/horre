// Created by Ethan McTague on 2019-09-25.

#include "Window.h"

Window::Window(glm::i64vec2 size, const char *title, IWindowUser *user) : size(size), user(user) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
    glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE);

    window = glfwCreateWindow(size.x, size.y, title, nullptr, nullptr);

    glfwSetWindowUserPointer(window, this);
    glfwSetCursorPosCallback(window, globalCursorPosCallback);
    glfwSetWindowSizeCallback(window, globalWindowSizeCallback);
    glfwSwapInterval(1);

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
}

Window::~Window() {
    glfwDestroyWindow(window);
    glfwTerminate();
}


/* Window manipulation calls */

void Window::setCursorEnabled(bool enabled) {
    glfwSetInputMode(window, GLFW_CURSOR, enabled ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}

void Window::mainLoop() {
    glfwMakeContextCurrent(window);

    // Force a resize to ensure perspective is set correctly
    user->onResize(size);

    auto previousTime = static_cast<float>(glfwGetTime());

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Get the current time in order to calculate delta
        auto currentTime = static_cast<float>(glfwGetTime());

        user->update(currentTime - previousTime, mouseEventDelta);
        user->render();

        // Update the previous time and reset the mouse movement value to zero
        previousTime = currentTime;
        mouseEventDelta = {0, 0};

        glfwSwapBuffers(window);
    }
}


/* Window-specific event handlers */

void Window::cursorPosCallback(glm::vec2 pos) {
    if (mouseHasMoved) {
        // If the mouse has moved previously, we need to register this movement as a real change in position
        mouseEventDelta = {pos.x - previousMousePosition.x, previousMousePosition.y - pos.y};
    } else {
        // If this is the first mouse movement event, we only need to indicate that the mouse has moved at least once
        mouseHasMoved = true;
    }

    previousMousePosition = pos;
}

void Window::windowSizeCallback(glm::i64vec2 newSize) {
    size = newSize;
    user->onResize(size);
}


/* Global window event handlers - responsible for invoking the window-specific version. */

void Window::globalCursorPosCallback(GLFWwindow *window, double posX, double posY) {
    static_cast<Window *>(glfwGetWindowUserPointer(window))->cursorPosCallback(glm::vec2{posX, posY});
}

void Window::globalWindowSizeCallback(GLFWwindow *window, int sizeX, int sizeY) {
    static_cast<Window *>(glfwGetWindowUserPointer(window))->windowSizeCallback(glm::i64vec2{sizeX, sizeY});
}
