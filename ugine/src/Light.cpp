#include "Light.h"
#include "State.h"
#include <sstream>
#include <iostream>

Light::Light() : _type(Type::DIRECTIONAL), _color(1.0f, 1.0f, 1.0f), _linearAttenuation(0.2f) {}
Light::~Light() {}

Type Light::getType() const {
    return _type;
}
void Light::setType(Type type) {
    _type = type;
}
const glm::vec3& Light::getColor() const {
    return _color;
}
void Light::setColor(const glm::vec3& color) {
    _color = color;
}
float Light::getLinearAttenuation() const {
    return _linearAttenuation;
}
void Light::setLinearAttenuation(float att) {
    _linearAttenuation = att;
}


void Light::prepare(LightAttribsLocation locations, std::shared_ptr<Shader>& shader) const {

#define SCALE(m) glm::scale(m, _scale)
#define ROTATE(m) glm::rotate(m, glm::angle(_rotation), glm::axis(_rotation))
#define TRANSLATE(m) glm::translate(m, _position)

    shader->setVec3(locations._locColor, _color);
    shader->setVec4(locations._locVector, State::viewMatrix * SCALE(ROTATE(TRANSLATE(glm::mat4()))) * glm::vec4(1, 1, 1, _type));
    shader->setFloat(locations._locLinearAttenuation, _linearAttenuation);
}