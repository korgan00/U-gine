#ifdef _MSC_VER
//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

#include "../lib/glew/glew.h"
#include "../lib/glfw3/glfw3.h"
#include "common.h"
#include "State.h"
#include "Shader.h"
#include "Vertex.h"
#include "Buffer.h"
#include "World.h"
#include "Entity.h"
#include "Model.h"
#include "Mesh.h"
#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <memory>

#define FULLSCREEN false

void initStates();
GLFWwindow* initGLFW();
bool init();


std::shared_ptr<World> createWorld(std::shared_ptr<Camera> mainCamera);
std::shared_ptr<Camera> createMainCamera();
std::shared_ptr<Shader> createBasicShader();
void updateMainCameraViewport(std::shared_ptr<Camera> mainCamera, GLFWwindow* window);

Vertex triangleVert[3] = {  glm::vec3( 0.0f,  1.0f, 0.0f),
							glm::vec3(-1.0f, -1.0f, 0.0f),
							glm::vec3( 1.0f, -1.0f, 0.0f)
						 };

GLushort triangleIdx[3] = { 0, 1, 2 };

GLint Z_POS[3] = {  0, -3, -6 };
GLint X_POS[3] = { -3,  0,  3 };

int main(int, char**) {
	GLFWwindow* window = initGLFW();

    if (!window) { return -1; }
    if (!init()) { return -1; }


    std::shared_ptr<Shader> shader = createBasicShader();
	if (!shader) { return -1; }

    State::defaultShader = shader;

    std::shared_ptr<Camera> mainCamera = createMainCamera();
    std::shared_ptr<World> world = createWorld(mainCamera);

	float lastTime = static_cast<float>(glfwGetTime());
	while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE)) {
		// update delta time
		float newTime = static_cast<float>(glfwGetTime());
		float deltaTime = newTime - lastTime;
		lastTime = newTime;

        updateMainCameraViewport(mainCamera, window);

        world->update(deltaTime);
        world->draw();

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

GLFWwindow* initGLFW() {
    if (glfwInit() != GLFW_TRUE) {
        std::cout << "could not initalize glfw" << std::endl;
        return nullptr;
    }
    atexit(glfwTerminate);

    // create window
    // glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    GLFWwindow* window = glfwCreateWindow(800, 600, "U-gine", FULLSCREEN ? glfwGetPrimaryMonitor() : nullptr, nullptr);
    if (!window) {
        std::cout << "could not create glfw window" << std::endl;
        return nullptr;
    }
    glfwMakeContextCurrent(window);
    return window;
}

std::shared_ptr<World> createWorld(std::shared_ptr<Camera> mainCamera) {
    std::shared_ptr<World> world = std::make_shared<World>();
    world->addEntity(mainCamera);

    std::shared_ptr<Buffer> triangleBuffer = std::make_shared<Buffer>(triangleVert, sizeof(triangleVert) / sizeof(Vertex),
                                                                      triangleIdx, sizeof(triangleIdx) / sizeof(GLushort));
    std::shared_ptr<Mesh> triangleMesh = std::make_shared<Mesh>();
    triangleMesh->addBuffer(triangleBuffer);

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            std::shared_ptr<Model> triangle = std::make_shared<Model>(triangleMesh);
            triangle->setPosition(glm::vec3(X_POS[i], 0.0f, Z_POS[j]));
            triangle->setRotation(glm::quat());
            triangle->setScale(glm::vec3(1.0f, 1.0f, 1.0f));

            world->addEntity(triangle);
        }
    }

    return world;
}

std::shared_ptr<Camera> createMainCamera() {
    std::shared_ptr<Camera> mainCamera = std::make_shared<Camera>();
    mainCamera->setClearColor(glm::vec3(0.0f, 0.0f, 0.0f));
    mainCamera->setPosition(glm::vec3(0.0f, 0.0f, 6.0f));
    mainCamera->setRotation(glm::quat());
    
    return mainCamera;
}

void updateMainCameraViewport(std::shared_ptr<Camera> mainCamera, GLFWwindow* window) {
    int screenWidth, screenHeight;
    glfwGetWindowSize(window, &screenWidth, &screenHeight);
    mainCamera->setProjection(glm::perspective(glm::half_pi<float>(), screenWidth / (float)screenHeight, 0.1f, 100.0f));
    mainCamera->setViewport(glm::ivec4(0, 0, screenWidth, screenHeight));

    std::stringstream ss;
    ss << screenWidth << "x" << screenHeight;
    glfwSetWindowTitle(window, ss.str().c_str());
}

std::shared_ptr<Shader> createBasicShader() {
    // read shaders
    std::string vertexShader = readString("data/shader.vert");
    std::string fragmentShader = readString("data/shader.frag");
    if (vertexShader == "" || fragmentShader == "") {
        std::cout << "could not load shaders" << std::endl;
        return nullptr;
    }

    const char* vsCode = vertexShader.c_str();
    const char* fsCode = fragmentShader.c_str();
    std::shared_ptr<Shader> s = Shader::createShader(vsCode, fsCode);
    if (Shader::getError() != "") {
        std::cout << s->getError() << std::endl;
        return nullptr;
    }
    return s;
}