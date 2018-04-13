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

	Camera(glm::ivec2 viewportSize) : _viewport(0, 0, viewportSize.x, viewportSize.y), _clrColor(0.0f, 0.0f, 0.0f) {
		_projection = glm::perspective(glm::pi<float>() / 3.0f, viewportSize.x / static_cast<float>(viewportSize.y), 0.1f, 100.0f);
	}

	const glm::mat4& getProjection() const;
	void setProjection(const glm::mat4& proj);

	const glm::ivec4& getViewport() const;
	void setViewport(const glm::ivec4& vp);

	const glm::vec3& getClearColor() const;
	void setClearColor(const glm::vec3& color);

	void prepare();
};
