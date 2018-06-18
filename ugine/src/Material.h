#pragma once

#include <memory>
#include "Texture.h"
#include "Shader.h"
#include "LightAttribsLocation.h"
#include "BlendMode.h"

class Material {
private:
    std::shared_ptr<Shader> _shader;
    std::shared_ptr<Texture> _tex;
    std::shared_ptr<Texture> _normalTex;
    std::shared_ptr<Texture> _shadowTex;
    std::shared_ptr<Texture> _reflectionTex;
    std::shared_ptr<Texture> _refractionTex;

    glm::vec4 _color;
    GLubyte _shininess;
    float _refractionCoef;

    BlendMode _blendMode;
    bool _lighting;
    bool _culling;
    bool _depthWrite;

    GLint _locMVP;
    GLint _locMV;
    GLint _locM;
    GLint _locEyePos;
    GLint _locNormalMatrix;

    GLint _locIsTexturized;
    GLint _locTexture;
    GLint _locHasNormalTex;
    GLint _locNormalTex;
    GLint _locHasReflectionTex;
    GLint _locReflectionTex;
    GLint _locHasRefractionTex;
    GLint _locRefractionTex;
    GLint _locIsCubeMap;
    GLint _locCubeTex;
    GLint _locCubeNormalTex;

    GLint _locShadowTex;
    GLint _locUseShadows;

    GLint _locColor;
    GLint _locShininess;
    GLint _locRefractionCoef;
    GLint _locNumLights;
    GLint _locAmbient;

	LightAttribsLocation _locLightAttribs[8];

    void prepareShaderAttributes();
    void prepareGlStates();

public:
    Material(const std::shared_ptr<Texture>& tex = nullptr,
             const std::shared_ptr<Shader>& shader = nullptr);

    const std::shared_ptr<Shader>& getShader() const;
    std::shared_ptr<Shader>& getShader();

    void setShader(const std::shared_ptr<Shader>& shader);

    const std::shared_ptr<Texture>& getTexture() const;
    void setTexture(const std::shared_ptr<Texture>& tex);

    const std::shared_ptr<Texture>& getNormalTexture() const;
    void setNormalTexture(const std::shared_ptr<Texture>& tex);

    const std::shared_ptr<Texture>& getReflectionTexture() const;
    void setReflectionTexture(const std::shared_ptr<Texture>& tex);

    const std::shared_ptr<Texture>& getRefractionTexture() const;
    void setRefractionTexture(const std::shared_ptr<Texture>& tex);

    const glm::vec4& getColor() const;
    void setColor(const glm::vec4& color);

    GLubyte getShininess() const;
    void setShininess(GLubyte shininess);

    float getRefractionCoef() const;
    void setRefractionCoef(float coef);

    BlendMode	getBlendMode() const;
    void		setBlendMode(BlendMode blendMode);

    bool		getLighting() const;
    void		setLighting(bool enable);

    bool		getCulling() const;
    void		setCulling(bool enable);

    bool		getDepthWrite() const;
    void		setDepthWrite(bool enable);

    void prepare();

};