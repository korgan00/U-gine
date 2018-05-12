#pragma once

#include <memory>
#include "Texture.h"
#include "Shader.h"
#include "LightAttribsLocation.h"

class Material {
private:
    std::shared_ptr<Shader> _shader;
    std::shared_ptr<Texture> _tex;

    glm::vec4 _color;
    GLubyte _shininess;

    GLint _locMVP;
    GLint _locMV;
    GLint _locNormalMatrix;
    GLint _locIsTexturized;
    GLint _locTexture;
    GLint _locColor;
    GLint _locShininess;
    GLint _locNumLights;
    GLint _locAmbient;

	LightAttribsLocation _locLightAttribs[8];

public:
    Material(const std::shared_ptr<Texture>& tex = nullptr,
             const std::shared_ptr<Shader>& shader = nullptr);

    const std::shared_ptr<Shader>& getShader() const;
    std::shared_ptr<Shader>& getShader();

    void setShader(const std::shared_ptr<Shader>& shader);

    const std::shared_ptr<Texture>& getTexture() const;
    void setTexture(const std::shared_ptr<Texture>& tex);

    const glm::vec4& getColor() const;
    void setColor(const glm::vec4& color);

    GLubyte getShininess() const;
    void setShininess(GLubyte shininess);

    void prepare();

};