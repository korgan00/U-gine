#pragma once

#include "Shader.h"
#include "Vertex.h"


class Buffer {
	GLuint _vbo = 0;
	GLuint _ebo = 0;
	GLuint _vboSize = 0;
	GLuint _eboSize = 0;
public:

	Buffer(Vertex* vertexData, GLuint numVertex, GLushort* indexData, GLuint numIndex);
	~Buffer();
	void draw(const Shader& shader) const;
};