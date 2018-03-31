#include "Buffer.h"


Buffer::Buffer(Vertex* vertexData, size_t numVertex, GLushort* indexData, size_t numIndex) {
	_vboSize = numVertex;
	_eboSize = numIndex;

	glGenBuffers(1, &_vbo);
	glGenBuffers(1, &_ebo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * _vboSize, vertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * _eboSize, indexData, GL_STATIC_DRAW);
}

Buffer::~Buffer() {
	glDeleteBuffers(1, &_vbo);
	glDeleteBuffers(1, &_ebo);
}

void Buffer::draw(const Shader& shader) const {
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	shader.setupAttribs();
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(_eboSize), GL_UNSIGNED_SHORT, 0);
}
