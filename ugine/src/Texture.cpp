#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(int w, int h, GLuint id) : _size(w, h), _texId(id) {}
Texture::~Texture() {
    if (_texId != 0) {
        glDeleteTextures(1, &_texId);
    }
}

std::shared_ptr<Texture> Texture::load(const char* filename) {
    int width, height;
    stbi_set_flip_vertically_on_load(true);
    stbi_uc* image = stbi_load(filename, &width, &height, nullptr, 4);

    // break if load fails
    if (image == nullptr) {
        return nullptr;
    }

    // continue uploading to GPU
    GLuint texId;

    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    // free the image
    stbi_image_free(image);

    // end this
    std::shared_ptr<Texture> tex(new Texture(width, height, texId), [](Texture* t) { delete t; });
    return tex;
}

uint32_t Texture::getId() const {
    return _texId;
}

const glm::ivec2& Texture::getSize() const {
    return _size;
}

void Texture::bind() const {
    glBindTexture(GL_TEXTURE_2D, _texId);
}