#include "Billboard.h"
#include "State.h"


Vertex quadVert[4] = {
	{ glm::vec3(-0.5f,  0.5f, 0.5f), glm::vec2(0.0f, 1.0f) },
	{ glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f) },
	{ glm::vec3(0.5f, -0.5f, 0.5f), glm::vec2(1.0f, 0.0f) },
	{ glm::vec3(0.5f,  0.5f, 0.5f), glm::vec2(1.0f, 1.0f) }
};
GLushort quadIdx[6] = {
	0,  1,  2,  0,  2,  3
};

Billboard::Billboard(const Material& mat) : _material(mat), _size(1, 1), _spin(0) {
	_buffer = std::make_shared<Buffer>(quadVert, 4, quadIdx, 6);

	if (mat.getTexture()) {
		_size = mat.getTexture()->getSize();
	}
}

const Material& Billboard::getMaterial() const {
	return _material;
}
Material& Billboard::getMaterial() {
	return _material;
}

const glm::vec2& Billboard::getSize() const {
	return _size;
}
void Billboard::setSize(const glm::vec2& size) {
	_size = size;
}

float Billboard::getSpin() const {
	return _spin;
}
void Billboard::setSpin(float spin) {
	_spin = spin;
}

void Billboard::draw() {
	glm::mat4 model = glm::transpose(State::viewMatrix);
	model[0][3] = 0;
	model[1][3] = 0;
	model[2][3] = 0;

	glm::vec3 p = getPosition();
	model[3] = glm::vec4(p.x, p.y, p.z, 1.0f);

	model = glm::rotate(model, glm::radians(_spin), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(_size, 1.0f));
	State::modelMatrix = model;

	_material.prepare();
	_buffer->draw(*_material.getShader());
}