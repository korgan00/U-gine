#include "Emitter.h"
#include <algorithm>


Emitter::Emitter(const Material& mat, bool autofade) 
	: _material(mat), _autofade(autofade), _emittedRest(0.0f) {
}
void Emitter::setRateRange(float min, float max) {
	_rateRange.first = min;
	_rateRange.second = max;
}
void Emitter::setVelocityRange(const glm::vec3& min, const glm::vec3& max) {
	_velocityRange.first = min;
	_velocityRange.second = max;
}
void Emitter::setSpinVelocityRange(float min, float max) {
	_spinVelocityRange.first = min;
	_spinVelocityRange.second = max;
}
void Emitter::setScaleRange(float min, float max) {
	_scaleRange.first = min;
	_scaleRange.second = max;
}
void Emitter::setLifetimeRange(float min, float max) {
	_lifetimeRange.first = min;
	_lifetimeRange.second = max;
}
void Emitter::setColorRange(const glm::vec4& min, const glm::vec4& max) {
	_colorRange.first = min;
	_colorRange.second = max;
}
void Emitter::emit(bool enable) {
	_isEmitting = enable;
}
bool Emitter::isEmitting() {
	return _isEmitting;
}
void Emitter::update(float deltaTime) {
	
	if (_isEmitting) {
		float particlesToEmit = glm::linearRand(_rateRange.first, _rateRange.second) * deltaTime + _emittedRest;
		_emittedRest = particlesToEmit - (int)particlesToEmit;

		for (int i = 0; i < particlesToEmit; i++) {
			_material.setColor(glm::linearRand(_colorRange.first, _colorRange.second));
			_emittedParticles.push_back(
				Particle(_material,
					glm::linearRand(_velocityRange.first, _velocityRange.second),
					glm::linearRand(_spinVelocityRange.first, _spinVelocityRange.second),
					glm::linearRand(_lifetimeRange.first, _lifetimeRange.second),
					_autofade)
			);
			_emittedParticles.back().setSize(_emittedParticles.back().getSize() * glm::linearRand(_scaleRange.first, _scaleRange.second));
		}
	}

	for (Particle p : _emittedParticles) {
		p.update(deltaTime);
	}

	auto itEnd = std::remove_if(_emittedParticles.begin(), _emittedParticles.end(), [](Particle p) { return p.getRemainingLifetime() <= 0; });
	_emittedParticles.erase(itEnd, _emittedParticles.end());
}

void Emitter::draw() {
	for (Particle p : _emittedParticles) {
		p.draw();
	}
}
