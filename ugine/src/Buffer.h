#pragma once

#include "Shader.h"
#include "Vertex.h"


class Buffer {
	GLuint _vbo = 0;
	GLuint _ebo = 0;
    size_t _vboSize = 0;
    size_t _eboSize = 0;
public:

	Buffer(Vertex* vertexData, size_t numVertex, GLushort* indexData, size_t numIndex);
	~Buffer();
	void draw(const Shader& shader) const;
};