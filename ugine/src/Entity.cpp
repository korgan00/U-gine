
#include "Entity.h"


Entity::Entity() : _scale(1.0f, 1.0f, 1.0f) {

}

const glm::vec3& Entity::getPosition() const {
	return _position;
}

void Entity::setPosition(const glm::vec3& pos) {
	_position = pos;
}

const glm::quat& Entity::getRotation() const {
	return _rotation;
}

const void Entity::setRotation(const glm::quat& rot) {
	_rotation = rot;
}

const glm::vec3& Entity::getScale() const {
	return _scale;
}

void Entity::setScale(const glm::vec3& scale) {
	_scale = scale;
}

void Entity::move(const glm::vec3& vec) {
	_position += _rotation * vec;
}
void Entity::setUpdateCB(std::function<void(float)> updateFunc) {
	_updateCB = updateFunc;
}
