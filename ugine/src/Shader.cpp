#include "Shader.h"
#include "Vertex.h"

using namespace std;

#define CHECK_ERR if (_errorLog != nullptr) return;

char* Shader::_errorLog  = nullptr;

shared_ptr<Shader> Shader::createShader(const char* vsCode, const char* fsCode) {
	shared_ptr<Shader> shader(new Shader(vsCode, fsCode), [](Shader* s) { delete s; });
	if (Shader::_errorLog == nullptr) {
		return shader;
	}
	return nullptr;
}
Shader::~Shader() {
	if (_programId != 0) {
		glDeleteProgram(_programId);
	}
}

Shader::Shader(const char* vsCode, const char* fsCode) : _programId(0) {
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
	glDeleteShader(vsId);
	glDeleteShader(fsId);
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
    _locVPos = glGetAttribLocation(_programId, "vpos");
    _locUV = glGetAttribLocation(_programId, "vuv");
    _locNorm = glGetAttribLocation(_programId, "vnormal");
    _locTan = glGetAttribLocation(_programId, "vtangent");
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
	if (_locVPos == -1 || _locUV == -1) {
		return;
	}
    glEnableVertexAttribArray(_locVPos);
    glVertexAttribPointer(_locVPos, 3, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));
    glEnableVertexAttribArray(_locUV);
    glVertexAttribPointer(_locUV, 2, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, texCoords)));
    glEnableVertexAttribArray(_locNorm);
    glVertexAttribPointer(_locNorm, 3, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));
    glEnableVertexAttribArray(_locTan);
    glVertexAttribPointer(_locTan, 3, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, tangent)));
}

// Obtiene la localización de una variable uniform
GLint Shader::getLocation(const char* name) const {
	return glGetUniformLocation(_programId, name);
}

// Da valor a una variable uniform
void Shader::setInt(int loc, int val) {
	if (loc >= 0) glUniform1i(loc, val);
}
void Shader::setFloat(int loc, float val) {
	if (loc >= 0) glUniform1f(loc, val);
}
void Shader::setVec3(int loc, const glm::vec3& vec) {
	if (loc >= 0) glUniform3fv(loc, 1, glm::value_ptr(vec));
}
void Shader::setVec4(int loc, const glm::vec4& vec) {
	if (loc >= 0) glUniform4fv(loc, 1, glm::value_ptr(vec));
}
void Shader::setMatrix(int loc, const glm::mat4& matrix) {
	if (loc >= 0) glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
}