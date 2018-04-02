#pragma once

#include "common.h"
#include "../lib/glew/glew.h"
#include <memory>


class Shader {
protected:

	static char* _errorLog;

	GLuint _programId;
    GLuint _locVPos;
    GLuint _locUV;

	GLuint createSubshader(const char* code, GLuint type);
	void setErrorLength(GLuint length);

	Shader(const char* vsCode, const char* fsCode);
	~Shader();

public:

	static std::shared_ptr<Shader> createShader(const char* vsCode, const char* fsCode);


	// Devuelve el identificador de OpenGL del programa
	uint32_t	getId() const;

	// Obtiene el mensaje de error generado al compilar o enlazar
	static const char* getError();

	// Activa el uso de este programa
	void		use() const;

	// Activa la escritura de las variables attribute,
	// y especifica su formato
	void		setupAttribs() const;

	// Obtiene la localización de una variable uniform
	GLint		getLocation(const char* name) const;

	// Da valor a una variable uniform
	void		setInt(int loc, int val);
	void		setFloat(int loc, float val);
	void		setVec3(int loc, const glm::vec3& vec);
	void		setVec4(int loc, const glm::vec4& vec);
	void		setMatrix(int loc, const glm::mat4& matrix);

};