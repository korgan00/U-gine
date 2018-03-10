#include "Shader.h"

#define CHECK_ERR if (_errorLog != nullptr) return;

char* Shader::_errorLog  = nullptr;

shared_ptr<Shader> Shader::createShader(const char* vsCode, const char* fsCode) {
	shared_ptr<Shader> shader(new Shader(vsCode, fsCode), destroy);
	if (Shader::_errorLog == nullptr) {
		return shader;
	}
	return nullptr;
}
Shader::~Shader() {
	glDeleteProgram(_programId); // Don't leak the shader.
	if (_errorLog != nullptr) {
		delete _errorLog;
	}
}
void Shader::destroy(Shader* s) {
	delete s;
}

Shader::Shader(const char* vsCode, const char* fsCode) {
	if (_errorLog != nullptr) {
		delete _errorLog;
	}
	// create vertex shader
	GLuint vsId = createSubshader(vsCode, GL_VERTEX_SHADER); CHECK_ERR;
	GLuint fsId = createSubshader(fsCode, GL_FRAGMENT_SHADER); CHECK_ERR;

	int isLinked = GL_FALSE;
	// create program
	_programId = glCreateProgram();
	glAttachShader(_programId, vsId);
	glAttachShader(_programId, fsId);
	glLinkProgram(_programId);
	glGetProgramiv(_programId, GL_LINK_STATUS, &isLinked);
	if (isLinked == GL_FALSE) {
		GLint maxLength = 0;
		glGetProgramiv(_programId, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		setErrorLength(maxLength);
		glGetProgramInfoLog(_programId, maxLength, &maxLength, _errorLog);

		// Exit with failure.
		glDeleteProgram(_programId); // Don't leak the shader.
	}
	CHECK_ERR;
	_vposLoc = glGetAttribLocation(_programId, "vpos");

	glDeleteShader(vsId); // Don't leak the shader.
	glDeleteShader(fsId); // Don't leak the shader.
}

GLuint Shader::createSubshader(const char* code, GLuint type) {
	int isCompiled = GL_FALSE;
	uint32_t shaderId = glCreateShader(type);
	glShaderSource(shaderId, 1, &code, nullptr);
	glCompileShader(shaderId);
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE) {
		GLint maxLength = 0;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		setErrorLength(maxLength);
		glGetShaderInfoLog(shaderId, maxLength, &maxLength, _errorLog);

		// Exit with failure.
		glDeleteShader(shaderId); // Don't leak the shader.
		return -1;
	}
	return shaderId;
}

void Shader::setErrorLength(GLuint length) {
	if (_errorLog != nullptr) {
		delete _errorLog;
	}
	_errorLog = new char[length];
}

// Devuelve el identificador de OpenGL del programa
uint32_t Shader::getId() const {
	return _programId;
}

// Obtiene el mensaje de error generado al compilar o enlazar
const char* Shader::getError() {
	if (_errorLog == nullptr) {
		return "";
	}
	return _errorLog;
}

// Activa el uso de este programa
void Shader::use() const {
	glUseProgram(_programId);
}

// Activa la escritura de las variables attribute,
// y especifica su formato
void Shader::setupAttribs() const {
	glEnableVertexAttribArray(_vposLoc);
	glVertexAttribPointer(_vposLoc, 3, GL_FLOAT, false, sizeof(GLfloat) * 3, 0);
}

// Obtiene la localización de una variable uniform
GLint Shader::getLocation(const char* name) const {
	return glGetUniformLocation(_programId, name);
}

// Da valor a una variable uniform
void Shader::setInt(int loc, int val) {
	glUniform1i(loc, val);
}
void Shader::setFloat(int loc, float val) {
	glUniform1f(loc, val);
}
void Shader::setVec3(int loc, const glm::vec3& vec) {
	glUniform3fv(loc, 1, glm::value_ptr(vec));
}
void Shader::setVec4(int loc, const glm::vec4& vec) {
	glUniform4fv(loc, 1, glm::value_ptr(vec));
}
void Shader::setMatrix(int loc, const glm::mat4& matrix) {
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
}