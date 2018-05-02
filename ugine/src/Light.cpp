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


void Light::prepare(int index, std::shared_ptr<Shader>& shader) const {

#define SCALE(m) glm::scale(m, _scale)
#define ROTATE(m) glm::rotate(m, glm::angle(_rotation), glm::axis(_rotation))
#define TRANSLATE(m) glm::translate(m, _position)

    std::stringstream ss;
    ss << "lights[" << index << "].color";
    GLint colorLoc = shader->getLocation(ss.str().c_str());
    shader->setVec3(colorLoc, _color);
    
    ss.str("");
    ss << "lights[" << index << "].vector";
    GLint vectorLoc = shader->getLocation(ss.str().c_str());
    shader->setVec4(vectorLoc, State::viewMatrix * SCALE(ROTATE(TRANSLATE(glm::mat4()))) * glm::vec4(1, 1, 1, _type));

    ss.str("");
    ss << "lights[" << index << "].linearAttenuation";
    GLint attenLoc = shader->getLocation(ss.str().c_str());
    shader->setFloat(attenLoc, _linearAttenuation);
    
}