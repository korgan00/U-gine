#pragma once

#include <memory>
#include "Texture.h"
#include "Shader.h"

class Material {
private:
    std::shared_ptr<Shader> _shader;
    std::shared_ptr<Texture> _tex;

    GLint _locMVP;
    GLint _locIsTexturized;
    GLint _locTexture;
public:
    Material(const std::shared_ptr<Texture>& tex = nullptr,
             const std::shared_ptr<Shader>& shader = nullptr);

    const std::shared_ptr<Shader>& getShader() const;
    std::shared_ptr<Shader>& getShader();

    void setShader(const std::shared_ptr<Shader>& shader);

    const std::shared_ptr<Texture>& getTexture() const;
    void setTexture(const std::shared_ptr<Texture>& tex);

    void prepare();

};