#pragma once

#include "Entity.h"
#include "Material.h"
#include "Particle.h"
#include <vector>

class Emitter : public Entity {
private:

	Material _material;
	bool _autofade;
	bool _isEmitting;

	std::pair<float, float> _rateRange;
	std::pair<glm::vec3, glm::vec3> _velocityRange;
	std::pair<float, float> _spinVelocityRange;
	std::pair<float, float> _scaleRange;
	std::pair<float, float> _lifetimeRange;
	std::pair<glm::vec4, glm::vec4> _colorRange;

	std::vector<std::shared_ptr<Particle>> _emittedParticles;
	float _emittedRest;

public: 
	Emitter(const Material& mat, bool autofade = true);
	void			setRateRange(float min, float max);
	void			setVelocityRange(const glm::vec3& min, const glm::vec3& max);
	void			setSpinVelocityRange(float min, float max);
	void			setScaleRange(float min, float max);
	void			setLifetimeRange(float min, float max);
	void			setColorRange(const glm::vec4& min, const glm::vec4& max);
	void			emit(bool enable);
	bool			isEmitting();
	virtual void	update(float deltaTime) override;
	virtual void	draw() override;

};