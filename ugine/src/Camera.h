#pragma once

#include <memory>
#include "common.h"
#include "Entity.h"

class Camera : public Entity {
protected:

	glm::vec3 _clrColor;
	glm::ivec4 _viewport;
	glm::mat4 _projection;

public:

	const glm::mat4& getProjection() const;
	void setProjection(const glm::mat4& proj);

	const glm::ivec4& getViewport() const;
	void setViewport(const glm::ivec4& vp);

	const glm::vec3& getClearColor() const;
	void setClearColor(const glm::vec3& color);

	void prepare();
};
