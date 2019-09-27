#pragma once

#include <vector>
#include <string>
#include "glinc.h"
#include "Shader.h"
#include "Window.h"

struct FBFormats {
    std::string name;
    int internalFormat, format, type;
};

class Framebuffer {
public:
    Framebuffer();

    Framebuffer(Window *window, std::vector<FBFormats> formats);

    ~Framebuffer();

    void updateSize(Window *window);

    void bind() { glBindFramebuffer(GL_FRAMEBUFFER, buffer); }

    void blitDepthTo(Framebuffer &other, int width, int height) {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, buffer);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, other.buffer);
        glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    }

    void asTextures(Shader *shader);

private:
    unsigned buffer, rbo;
    std::vector<FBFormats> colorFormats;
    std::vector<unsigned> colorBuffers;
    std::vector<unsigned> colorAttachments;
};


