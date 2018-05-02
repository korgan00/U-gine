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

void Material::prepare() {
    glm::mat4 mv = State::viewMatrix * State::modelMatrix;
    std::shared_ptr<Shader> s = getShader();
    int numLights = State::lights.size();
    s->use();
    s->setMatrix(_locMVP, State::projectionMatrix * mv);
    s->setMatrix(_locMV, mv);
    s->setMatrix(_locNormalMatrix, glm::transpose(glm::inverse(mv)));
    s->setInt(_locTexture, 0);
    s->setInt(_locIsTexturized, _tex ? 1 : 0);
    s->setVec4(_locColor, _color);
    s->setInt(_locShininess, _shininess);
    s->setVec3(_locAmbient, State::ambient);
    s->setInt(_locNumLights, numLights);

    if (_tex) {
        _tex->bind();
    }
    
    for (int i = 0; i < State::lights.size(); ++i) {
        State::lights[i]->prepare(i, s);
    }
}