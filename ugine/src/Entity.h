#pragma once

#include "common.h"

class Entity {
protected:

	glm::vec3 _position;
	glm::quat _rotation;
	glm::vec3 _scale;

public:
	Entity();
	virtual ~Entity() {}

	const glm::vec3&	getPosition() const;
	void				setPosition(const glm::vec3& pos);
	const				glm::quat& getRotation() const;	// puede ser quat
	const void			setRotation(const glm::quat& rot);	// puede ser quat
	const glm::vec3&	getScale() const;
	void				setScale(const glm::vec3& scale);

	void				move(const glm::vec3& vec);	// Ver en las diapositivas

	virtual void		update(float deltaTime) {}
	virtual void		draw() {}
};