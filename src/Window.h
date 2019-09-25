#pragma once

#include "glinc.h"
#include <glm/glm.hpp>

/** Anything that ties into a window's main loop. */
class IWindowUser {
public:

    /** Handles state updating, event handling, etc. */
    virtual void update(float deltaTime, glm::vec2 mouseDelta) = 0;

    /** Handles rendering to the screen. */
    virtual void render() = 0;

    /** Handles resize events. */
    virtual void onResize(glm::i64vec2 size) = 0;
};


class Window {
public:

    /** Initialize a window with the given size and title. */
    Window(glm::i64vec2 size, const char *title, IWindowUser *user);

    /** Handle window cleanup. This will cleanup GLFW entirely. */
    ~Window();

    /** Sets the cursor to enabled or disabled (grabs cursor when disabled.) */
    void setCursorEnabled(bool enabled);

    /** Determines if a key is pressed. */
    bool keyIsDown(int key) {
        return glfwGetKey(window, key) == GLFW_PRESS;
    }


    /** Runs the main window update/draw loop. */
    void mainLoop();


private:

    /** Triggered when the mouse moves in this window. */
    void cursorPosCallback(glm::vec2 pos);

    /** Triggered when the window is resized. */
    void windowSizeCallback(glm::i64vec2 size);


    /** The GLFW window handle for this window. */
    GLFWwindow *window;

    /** The current size of the window. */
    glm::i64vec2 size;

    /** Indicates if the mouse has moved yet - important for handling mouse movement. */
    bool mouseHasMoved = false;

    /** Indicates the previous position of the mouse. */
    glm::vec2 previousMousePosition{0.0};

    /** Stores the movement of the mouse between updates. */
    glm::vec2 mouseEventDelta{0.0, 0.0};

    /** The window user for this window. */
    IWindowUser *user;


    /** Event handler triggered when a mouse moves in any window. */
    static void globalCursorPosCallback(GLFWwindow *window, double posX, double posY);

    /** Event handler triggered when any window is resized. */
    static void globalWindowSizeCallback(GLFWwindow *window, int sizeX, int sizeY);
};

