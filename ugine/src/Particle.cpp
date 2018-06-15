#include "Particle.h"
#include <algorithm>

Particle::Particle(const Material& mat, const glm::vec3& vel, float spinVel, float lifetime, bool autofade) 
	: Billboard(mat), _vel(vel), _spinVel(spinVel), _maxLifetime(lifetime), 
    _remainingLifetime(lifetime), _autofade(autofade) {

}

float Particle::getRemainingLifetime() const {
	return _remainingLifetime;
}
void Particle::update(float deltaTime) {
	_remainingLifetime = std::max(_remainingLifetime - deltaTime, 0.0f);
	setPosition(getPosition() + _vel * deltaTime);

	setRotation(glm::rotate(getRotation(), deltaTime * _spinVel, glm::vec3(0.0f,0.0f, 1.0f)));

    if (_autofade) {
        glm::vec4 color = getMaterial().getColor();
        color.a = _remainingLifetime / _maxLifetime;
        getMaterial().setColor(color);
    }
}
