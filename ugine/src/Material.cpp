#include "Material.h"
#include "State.h"


Material::Material( const std::shared_ptr<Texture>& tex,
                    const std::shared_ptr<Shader>& shader) 
    : _tex(tex) {
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
    _locIsTexturized = getShader()->getLocation("isTexturized");
    _locTexture = getShader()->getLocation("tex");
}
const std::shared_ptr<Texture>& Material::getTexture() const {
    return _tex;
}
void Material::setTexture(const std::shared_ptr<Texture>& tex) {
    _tex = tex;
}
void Material::prepare() {
    glm::mat4 mvp = State::projectionMatrix * State::viewMatrix * State::modelMatrix;
    std::shared_ptr<Shader> s = getShader();
    s->use();
    s->setMatrix(_locMVP, mvp);
    s->setInt(_locTexture, 0);
    s->setInt(_locIsTexturized, _tex ? 1 : 0);

    if (_tex) {
        _tex->bind();
    }
}