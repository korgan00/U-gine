#ifdef _MSC_VER
//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

#include "../lib/glew/glew.h"
#include "../lib/glfw3/glfw3.h"
#include "common.h"
#include "Shader.h"
#include "Vertex.h"
#include "Buffer.h"
#include "utils.h"
#include <array>
#include <fstream>
#include <iostream>
#include <sstream>

#define FULLSCREEN false

void initStates();
bool init();

Vertex triangleVert[3] = {  glm::vec3( 0.0f,  1.0f, 0.0f),
							glm::vec3(-1.0f, -1.0f, 0.0f),
							glm::vec3( 1.0f, -1.0f, 0.0f)
						 };

GLushort triangleIdx[3] = { 0, 1, 2 };

GLint Z_POS[3] = {  0, -3, -6 };
GLint X_POS[3] = { -3,  0,  3 };

int main(int, char**) {
	if ( glfwInit() != GLFW_TRUE ) {
		std::cout << "could not initalize glfw" << std::endl;
		return -1;
	}
	atexit(glfwTerminate);

	// create window
	// glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "U-gine", FULLSCREEN ? glfwGetPrimaryMonitor() : nullptr, nullptr);
	if (!window) {
		std::cout << "could not create glfw window" << std::endl;
		return -1;
	}
	glfwMakeContextCurrent(window);

	init();

	// read shaders
	std::string vertexShader = readString("data/shader.vert");
	std::string fragmentShader = readString("data/shader.frag");
	if (vertexShader == "" || fragmentShader == "") {
		std::cout << "could not load shaders" << std::endl;
		return -1;
	}

	const char* vsCode = vertexShader.c_str();
	const char* fsCode = fragmentShader.c_str();
	shared_ptr<Shader> s = Shader::createShader(vsCode, fsCode);
	if (Shader::getError() != "") {
		std::cout << s->getError() << std::endl;
		return -1;
	}
	s->use();
	GLint locMVP = s->getLocation("mvp");

	Buffer triangleBuffer(triangleVert, sizeof(triangleVert) / sizeof(Vertex), 
						  triangleIdx, sizeof(triangleIdx) / sizeof(GLushort));

	float rotationAngle = 0;

	float lastTime = static_cast<float>(glfwGetTime());
	while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE)) {
		// update delta time
		float newTime = static_cast<float>(glfwGetTime());
		float deltaTime = newTime - lastTime;
		lastTime = newTime;

		rotationAngle += 32 * deltaTime;

		// get updated screen size
		int screenWidth, screenHeight;
		glfwGetWindowSize(window, &screenWidth, &screenHeight);
		glm::mat4 matP = glm::perspective(glm::half_pi<float>(), screenWidth / (float)screenHeight, 0.1f, 100.0f);
		glm::mat4 matV = glm::lookAt(glm::vec3(0.0f, 0.0f, 6.0f), glm::vec3(), glm::vec3(0.0f, 1.0f, 0.0f));

		glm::mat4 matVP = matP*matV;

		// report screen size
		std::stringstream ss;
		ss << screenWidth << "x" << screenHeight;
		glfwSetWindowTitle(window, ss.str().c_str());

		// clear screen
		glViewport(0, 0, screenWidth, screenHeight);
		glScissor(0, 0, screenWidth, screenHeight);
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 matRot = glm::rotate(glm::mat4(), glm::radians(rotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
		// draw triangle
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				glm::mat4 matM = glm::translate(glm::mat4(), glm::vec3(X_POS[i], 0.0f, Z_POS[j])) * matRot;

				s->setMatrix(locMVP, matVP*matM);
				triangleBuffer.draw(*s);
			}
		}



		// update swap chain & process events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	return 0;
}

bool init() {
	// init glew
	if (glewInit()) {
		std::cout << "could not initialize glew" << std::endl;
		return false;
	}
	initStates();
	return true;
}

void initStates() {
	glEnable(GL_SCISSOR_TEST);
	glEnable(GL_DEPTH_TEST);
}
