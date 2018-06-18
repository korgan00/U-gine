#pragma once

#include "Texture.h"

class Framebuffer {
private:
    GLuint _fbId;
    std::shared_ptr<Texture> _colorTex;
    std::shared_ptr<Texture> _depthTex;

public:
    Framebuffer(
        const std::shared_ptr<Texture>& colorTex,
        const std::shared_ptr<Texture>& depthTex);
    ~Framebuffer();

    const std::shared_ptr<Texture>& getColorTexture() const;
    const std::shared_ptr<Texture>& getDepthTexture() const;

    void bind() const;
    static void bindScreen();

};