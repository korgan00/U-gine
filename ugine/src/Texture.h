#pragma once

#include <memory>
#include "../lib/glew/glew.h"
#include "common.h"

#define ALBEDO_LAYER 0
#define NORMALMAP_LAYER 1
#define REFLECTION_TEX_LAYER 2
#define REFRACTION_TEX_LAYER 3
#define CUBEMAP_ALBEDO_LAYER 4
#define CUBEMAP_NORMALMAP_LAYER 5
#define SHADOWMAP_LAYER 15

class Texture {
private:
    glm::ivec2 _size;
    GLuint _texId;
    bool _isCube;
    bool _isDepth;

    Texture(int w, int h, GLuint id, bool isCube = false);

public:
    Texture(glm::uvec2 size, bool isDepth = false);
    ~Texture();

    static std::shared_ptr<Texture>	load(const char* filename);
    static std::shared_ptr<Texture>	load(const char* left, const char* right,
                                         const char* front, const char* back,
                                         const char* top, const char* bottom);

    uint32_t					    getId() const;
    const glm::ivec2&				getSize() const;
    void						    bind(size_t layer = 0) const;
    bool isCube() const;
    bool isDepth() const;

};