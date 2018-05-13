#include "Material.h"
#include "State.h"


Material::Material( const std::shared_ptr<Texture>& tex,
                    const std::shared_ptr<Shader>& shader) : _tex(tex), _color(1.0f, 1.0f, 1.0f, 1.0f) {
    setShader(shader);
}

const std::shared_ptr<Shader>& Material::getShader() const {
    return _shader ? _shader : State::defaultShader;
}
std::shared_ptr<Shader>& Material::getShader() {
    return _shader ? _shader : State::defaultShader;
}
void Material::setShader(const std::shared_ptr<Shader>& shader) {
    _shader = shader;
    _locMVP = getShader()->getLocation("mvp");
    _locMV = getShader()->getLocation("mv");
    _locNormalMatrix = getShader()->getLocation("normalMat");
    _locIsTexturized = getShader()->getLocation("isTexturized");
    _locTexture = getShader()->getLocation("tex");
    _locColor = getShader()->getLocation("color");
    _locShininess = getShader()->getLocation("shininess");
    _locAmbient = getShader()->getLocation("ambient");
    _locNumLights = getShader()->getLocation("numLights");

	// get light properties location
	std::stringstream ss;
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

void Material::prepareShaderAttributes() {
    glm::mat4 mv = State::viewMatrix * State::modelMatrix;
    std::shared_ptr<Shader> s = getShader();
    s->use();
    s->setMatrix(_locMVP, State::projectionMatrix * mv);
    s->setMatrix(_locMV, mv);
    s->setMatrix(_locNormalMatrix, glm::transpose(glm::inverse(mv)));
    s->setInt(_locTexture, 0);
    s->setInt(_locIsTexturized, _tex ? 1 : 0);
    s->setVec4(_locColor, _color);
    s->setInt(_locShininess, _shininess);
    s->setVec3(_locAmbient, State::ambient);
    s->setInt(_locNumLights, _lighting ? State::lights.size() : 0);
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
    
    prepareShaderAttributes();

    prepareGlStates();

    if (_tex) {
        _tex->bind();
    }

    if (_lighting) {
        for (int i = 0; i < State::lights.size(); ++i) {
            State::lights[i]->prepare(_locLightAttribs[i], getShader());
        }
    }
}