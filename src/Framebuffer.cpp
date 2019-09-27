// Created by Ethan McTague on 2019-09-27.

#include <iostream>
#include "Framebuffer.h"

Framebuffer::Framebuffer(Window *window, std::vector<FBFormats> formats) : buffer(0), colorFormats(formats) {
    updateSize(window);
}

Framebuffer::~Framebuffer() {
    glDeleteTextures(colorBuffers.size(), colorBuffers.data());
    glDeleteRenderbuffers(1, &rbo);
    glDeleteFramebuffers(1, &buffer);
}

void Framebuffer::asTextures(Shader *shader) {
    for (int i = 0; i < colorBuffers.size(); i++) {
        shader->uniform(colorFormats[i].name, i);
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
    }
}

Framebuffer::Framebuffer() : buffer(0) {

}

void Framebuffer::updateSize(Window *window) {
    auto fbSize = window->getFramebufferSize();

    if (buffer != 0) {
        glDeleteFramebuffers(1, &buffer);
        glDeleteTextures(colorBuffers.size(), colorBuffers.data());
        glDeleteRenderbuffers(1, &rbo);
        colorBuffers.clear();
        colorAttachments.clear();
    }

    glGenFramebuffers(1, &buffer);
    glBindFramebuffer(GL_FRAMEBUFFER, buffer);

    int i = 0;
    for (auto format : colorFormats) {
        unsigned texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, format.internalFormat, fbSize.x, fbSize.y, 0, format.format, format.type, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, texture, 0);
        colorBuffers.push_back(texture);
        colorAttachments.push_back(GL_COLOR_ATTACHMENT0 + i++);
    }

    glDrawBuffers(colorAttachments.size(), colorAttachments.data());

    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, fbSize.x, fbSize.y);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        throw std::runtime_error("Framebuffer was not completed.");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
