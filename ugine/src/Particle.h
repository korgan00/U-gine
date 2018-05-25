#pragma once

#include "Billboard.h"

class Particle : public Billboard {
private:
	glm::vec3 _vel;
	float _spinVel;
	float _maxLifetime;
	float _remainingLifetime;
	float _opacity;
	bool _autofade;

public:

	Particle(const Material& mat, const glm::vec3& vel, float spinVel, float lifetime, bool autofade);

	float			getRemainingLifetime() const;
	virtual void	update(float deltaTime) override;

};
