#pragma once

#include <memory>
#include "../lib/glew/glew.h"
#include "common.h"

class Texture {
private:
    glm::ivec2 _size;
    GLuint _texId;
    bool _isCube;

    Texture(int w, int h, GLuint id, bool isCube = false);
    ~Texture();

public:
    static std::shared_ptr<Texture>	load(const char* filename);
    static std::shared_ptr<Texture>	load(const char* left, const char* right,
                                         const char* front, const char* back,
                                         const char* top, const char* bottom);

    uint32_t					    getId() const;
    const glm::ivec2&				getSize() const;
    void						    bind(size_t layer = 0) const;
    bool isCube() const;
};