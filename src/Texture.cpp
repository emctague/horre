// Created by Ethan McTague on 2019-09-26.

#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION

#include "../stb/stb_image.h"
#include <iostream>

Texture::Texture(const std::string &path) {
    // Load the texture with stb_image
    int width, height, channelCount;
    unsigned char *data = stbi_load(("../res/texture/" + path).c_str(), &width, &height, &channelCount, 0);
    if (!data) throw std::runtime_error("Could not load texture: " + path);

    // Create and populate a texture
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
}

Texture::~Texture() {
    glDeleteTextures(1, &texture);
}
