#include "Material.h"
#include "State.h"
#include <iostream>
#include <algorithm>

Material::Material( const std::shared_ptr<Texture>& tex,
                    const std::shared_ptr<Shader>& shader) 
	: _tex(tex), _color(1.0f, 1.0f, 1.0f, 1.0f), _lighting(true), 
	_culling(true), _depthWrite(true), _blendMode(ALPHA) {
    setShader(shader);
}

const std::shared_ptr<Shader>& Material::getShader() const {
    return _shader ? _shader : State::defaultShader;
}
std::shared_ptr<Shader>& Material::getShader() {
    return _shader ? _shader : State::defaultShader;
}
void Material::setShader(const std::shared_ptr<Shader>& shader) {
    std::stringstream ss;
    _shader = shader;
    
    // Matrix and global data
    _locMVP = getShader()->getLocation("mvp");
    _locMV = getShader()->getLocation("mv");
    _locM = getShader()->getLocation("m");
    _locDepthBiasMatrix = getShader()->getLocation("depthBiasMatrix");
    _locNormalMatrix = getShader()->getLocation("normalMat");
    _locEyePos = getShader()->getLocation("eyePos");

    // Animation
    _locSkinned = getShader()->getLocation("skinned");
    for (int i = 0; i < MAX_ANIM_MATRICES_COUNT; i++) {
        ss.str("");
        ss << "animMatrices[" << i << "]";
        _locAnimMatrices[i] = getShader()->getLocation(ss.str().c_str());
    }
    // Textures
    _locIsTexturized = getShader()->getLocation("isTexturized");
    _locTexture = getShader()->getLocation("tex");
    _locHasNormalTex = getShader()->getLocation("hasNormalTex");
    _locNormalTex = getShader()->getLocation("normalTex");
    _locHasReflectionTex = getShader()->getLocation("hasReflectionTex");
    _locReflectionTex = getShader()->getLocation("reflectionTex");
    _locHasRefractionTex = getShader()->getLocation("hasRefractionTex");
    _locRefractionTex = getShader()->getLocation("refractionTex");
    _locIsCubeMap = getShader()->getLocation("isCubeMap");
    _locCubeTex = getShader()->getLocation("cubeTex");
    _locCubeNormalTex = getShader()->getLocation("cubeNormalTex");

    // Shadows
    _locShadowTex = getShader()->getLocation("shadowDepthMap");
    _locUseShadows = getShader()->getLocation("useShadows");

    // Material properties
    _locColor = getShader()->getLocation("color");
    _locShininess = getShader()->getLocation("shininess");
    _locRefractionCoef = getShader()->getLocation("refractionCoef");
    _locAmbient = getShader()->getLocation("ambient");

	// Light properties location
    _locNumLights = getShader()->getLocation("numLights");
	for (int i = 0; i < 8; i++) {
		ss.str("");
		ss << "lights[" << i << "].color";
		_locLightAttribs[i]._locColor = getShader()->getLocation(ss.str().c_str());

		ss.str("");
		ss << "lights[" << i << "].vector";
		_locLightAttribs[i]._locVector = getShader()->getLocation(ss.str().c_str());

		ss.str("");
		ss << "lights[" << i << "].linearAttenuation";
		_locLightAttribs[i]._locLinearAttenuation = getShader()->getLocation(ss.str().c_str());
	}
}
const std::shared_ptr<Texture>& Material::getTexture() const {
    return _tex;
}
void Material::setTexture(const std::shared_ptr<Texture>& tex) {
    _tex = tex;
}

const glm::vec4& Material::getColor() const {
    return _color;
}
void Material::setColor(const glm::vec4& color) {
    _color = color;
}

GLubyte Material::getShininess() const {
    return _shininess;
}
void Material::setShininess(GLubyte shininess) {
    _shininess = shininess;
}

BlendMode Material::getBlendMode() const {
    return _blendMode;
}
void Material::setBlendMode(BlendMode blendMode) {
    _blendMode = blendMode;
}

bool Material::getLighting() const {
    return _lighting;
}
void Material::setLighting(bool enable) {
    _lighting = enable;
}

bool Material::getCulling() const {
    return _culling;
}
void Material::setCulling(bool enable) {
    _culling = enable;
}

bool Material::getDepthWrite() const {
    return _depthWrite;
}
void Material::setDepthWrite(bool enable) {
    _depthWrite = enable;
}

const std::shared_ptr<Texture>& Material::getReflectionTexture() const {
    return _reflectionTex;
}
void Material::setReflectionTexture(const std::shared_ptr<Texture>& tex) {
    _reflectionTex = tex;
}

const std::shared_ptr<Texture>& Material::getRefractionTexture() const {
    return _refractionTex;
}
void Material::setRefractionTexture(const std::shared_ptr<Texture>& tex) {
    _refractionTex = tex;
}

const std::shared_ptr<Texture>& Material::getNormalTexture() const {
    return _normalTex;
}
void Material::setNormalTexture(const std::shared_ptr<Texture>& tex) {
    _normalTex = tex;
}

float Material::getRefractionCoef() const {
    return _refractionCoef;
}
void Material::setRefractionCoef(float coef) {
    _refractionCoef = coef;
}

void Material::prepareShaderAttributes() {
    glm::mat4 mv = State::viewMatrix * State::modelMatrix;
    std::shared_ptr<Shader> s = getShader();
    s->use();
    s->setMatrix(_locMVP, State::projectionMatrix * mv);
    s->setMatrix(_locMV, mv);
    s->setMatrix(_locM, State::modelMatrix);
    s->setMatrix(_locNormalMatrix, glm::transpose(glm::inverse(mv)));
    s->setMatrix(_locDepthBiasMatrix, State::depthBiasMatrix);
    s->setVec3(_locEyePos, State::eyePos);

    s->setInt(_locSkinned, State::animation ? 1 : 0);
    if (State::animation) {
        size_t matricesCount = std::min(State::animMatrices->size(), static_cast<size_t>(MAX_ANIM_MATRICES_COUNT));
        for (int i = 0; i < matricesCount; i++) {
            s->setMatrix(_locAnimMatrices[i], State::animMatrices->at(i));
        }
    }

    s->setInt(_locTexture, ALBEDO_LAYER);
    s->setInt(_locNormalTex, NORMALMAP_LAYER);
    s->setInt(_locReflectionTex, REFLECTION_TEX_LAYER);
    s->setInt(_locRefractionTex, REFRACTION_TEX_LAYER);
    s->setInt(_locCubeTex, CUBEMAP_ALBEDO_LAYER);
    s->setInt(_locCubeNormalTex, CUBEMAP_NORMALMAP_LAYER);
    s->setInt(_locShadowTex, SHADOWMAP_LAYER);
    s->setInt(_locIsTexturized, _tex ? 1 : 0);
    s->setInt(_locHasNormalTex, _normalTex ? 1 : 0);
    s->setInt(_locHasReflectionTex, _reflectionTex ? 1 : 0);
    s->setInt(_locHasRefractionTex, _refractionTex ? 1 : 0);
    s->setInt(_locIsCubeMap, _tex && _tex->isCube() ? 1 : 0);
    s->setInt(_locUseShadows, State::shadows);

    s->setVec4(_locColor, _color);
    s->setInt(_locShininess, _shininess);
    s->setFloat(_locRefractionCoef, _refractionCoef);
    s->setVec3(_locAmbient, State::ambient);
    s->setInt(_locNumLights, _lighting ? State::lights.size() : static_cast<size_t>(0));
}

void Material::prepareGlStates() {
    if (_blendMode == BlendMode::ALPHA) {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    } else if (_blendMode == BlendMode::ADD) {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    } else if (_blendMode == BlendMode::MUL) {
        glBlendFunc(GL_ZERO, GL_SRC_COLOR);
    }

    if (_culling) {
        glEnable(GL_CULL_FACE);
    } else {
        glDisable(GL_CULL_FACE);
    }

    glDepthMask(_depthWrite ? GL_TRUE : GL_FALSE);
}

void Material::prepare() {
    prepareGlStates();

    if (State::overrideShader) {
        State::overrideShader->use();
        State::overrideShader->setMatrix(
            State::overrideShader->getLocation("mvp"),
            State::projectionMatrix * State::viewMatrix * State::modelMatrix);
    } else {
        prepareShaderAttributes();

        if (_tex) {
            _tex->bind(_tex->isCube() ? CUBEMAP_ALBEDO_LAYER : ALBEDO_LAYER);
        }
        if (_normalTex) {
            _normalTex->bind(_normalTex->isCube() ? CUBEMAP_NORMALMAP_LAYER : NORMALMAP_LAYER);
        }
        if (_reflectionTex) { // is supposed to be a cube
            _reflectionTex->bind(REFLECTION_TEX_LAYER);
        }
        if (_refractionTex) { // is supposed to be a cube
            _refractionTex->bind(REFRACTION_TEX_LAYER);
        }

        if (_lighting) {
            for (int i = 0; i < State::lights.size(); ++i) {
                State::lights[i]->prepare(_locLightAttribs[i], getShader());
            }
        }
    }
}