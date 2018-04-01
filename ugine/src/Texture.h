#pragma once

#include <memory>
#include "../lib/glew/glew.h"
#include "common.h"

class Texture {
private:
    glm::ivec2 _size;
    GLuint _texId;

    Texture(int w, int h, GLuint id);

public:
    static std::shared_ptr<Texture>	load(const char* filename);
    uint32_t					    getId() const;
    const glm::ivec2&				getSize() const;
    void						    bind() const;
};