#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(int w, int h, GLuint id, bool isCube) : _size(w, h), _texId(id), _isCube(isCube), _isDepth(false) {}
Texture::~Texture() {
    if (_texId != 0) {
        glDeleteTextures(1, &_texId);
    }
}

Texture::Texture(glm::uvec2 size, bool isDepth) : _size(size), _isCube(false), _isDepth(isDepth) {
    glGenTextures(1, &_texId);
    glBindTexture(GL_TEXTURE_2D, _texId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, isDepth ? GL_DEPTH_COMPONENT24 : GL_RGBA, size.x, size.y, 0, isDepth ? GL_DEPTH_COMPONENT : GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
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

std::shared_ptr<Texture> Texture::load(const char* left, const char* right,
                                       const char* front, const char* back,
                                       const char* top, const char* bottom) {
    int width, height;
    stbi_set_flip_vertically_on_load(false);
    stbi_uc* imageLeft = stbi_load(left, &width, &height, nullptr, 4);
    stbi_uc* imageRight = stbi_load(right, &width, &height, nullptr, 4);
    stbi_uc* imageFront = stbi_load(front, &width, &height, nullptr, 4);
    stbi_uc* imageBack = stbi_load(back, &width, &height, nullptr, 4);
    stbi_uc* imageTop = stbi_load(top, &width, &height, nullptr, 4);
    stbi_uc* imageBottom = stbi_load(bottom, &width, &height, nullptr, 4);

    // break if load fails
    if (imageLeft == nullptr || imageRight == nullptr || imageFront == nullptr ||
        imageBack == nullptr || imageTop == nullptr || imageBottom == nullptr) {
        return nullptr;
    }

    // continue uploading to GPU
    GLuint texId;

    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texId);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageRight);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageLeft);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageFront);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageBack);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageTop);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageBottom);
    //glGenerateMipmap(GL_TEXTURE_2D);

    // free the image
    stbi_image_free(imageLeft);
    stbi_image_free(imageRight);
    stbi_image_free(imageFront);
    stbi_image_free(imageBack);
    stbi_image_free(imageTop);
    stbi_image_free(imageBottom);

    // end this
    std::shared_ptr<Texture> tex(new Texture(width, height, texId, true), [](Texture* t) { delete t; });
    return tex;
}

uint32_t Texture::getId() const {
    return _texId;
}

const glm::ivec2& Texture::getSize() const {
    return _size;
}

void Texture::bind(size_t layer) const {
    glActiveTexture(GL_TEXTURE0 + layer);
    if (_isCube) {
        glBindTexture(GL_TEXTURE_CUBE_MAP, _texId);
    } else {
        glBindTexture(GL_TEXTURE_2D, _texId);
    }
}

bool Texture::isCube() const {
    return _isCube;
}
bool Texture::isDepth() const {
    return _isDepth;
}