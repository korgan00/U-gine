#pragma once

#include "common.h"
#include <functional>

class Entity {
protected:
    static unsigned int currendId;
    unsigned int _id;

	glm::vec3 _position;
	glm::quat _rotation;
	glm::vec3 _scale;

	std::function<void(float)> _updateCB;

public:
	Entity();
	virtual ~Entity() {}

	const glm::vec3&	getPosition() const;
	void				setPosition(const glm::vec3& pos);
	const				glm::quat& getRotation() const;	// puede ser quat
	const void			setRotation(const glm::quat& rot);	// puede ser quat
	const glm::vec3&	getScale() const;
	void				setScale(const glm::vec3& scale);
	void				setUpdateCB(const std::function<void(float)> updateFunc);

	void				move(const glm::vec3& vec);	// Ver en las diapositivas

	virtual void		update(float deltaTime) { if (_updateCB) _updateCB(deltaTime); }
	virtual void		draw() {}
};