#pragma once

#include <string>
#include "glinc.h"

/// OpenGL texture instance
class Texture {
public:
    /// Create a texture from the given file
    explicit Texture(const std::string &path);

    ~Texture();

    /// Bind this texture to a texture unit
    void use(int index) {
        glActiveTexture(GL_TEXTURE0 + index);
        glBindTexture(GL_TEXTURE_2D, texture);
    }

private:

    /// The OpenGL shader program instance being used
    unsigned texture;
};


