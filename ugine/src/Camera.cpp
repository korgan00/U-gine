
#include "Camera.h"
#include "State.h"


const glm::mat4& Camera::getProjection() const {
	return _projection;
}
void Camera::setProjection(const glm::mat4& proj) {
	_projection = proj;
}

const glm::ivec4& Camera::getViewport() const {
	return _viewport;
}
void Camera::setViewport(const glm::ivec4& vp) {
	_viewport = vp;
}

const glm::vec3& Camera::getClearColor() const {
	return _clrColor;
}
void Camera::setClearColor(const glm::vec3& color) {
	_clrColor = color;
}

void Camera::prepare() {
	State::projectionMatrix = _projection;
    State::eyePos = getPosition();

#define ROTATE(m) glm::rotate(m, -glm::angle(_rotation), glm::axis(_rotation))
#define TRANSLATE(m) glm::translate(m, -_position)

	State::viewMatrix = TRANSLATE(ROTATE(glm::mat4()));
    glDepthMask(true);
	glViewport(_viewport.x, _viewport.y, _viewport.z, _viewport.w);
	glScissor(_viewport.x, _viewport.y, _viewport.z, _viewport.w);
	glClearColor(_clrColor.r, _clrColor.g, _clrColor.b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}