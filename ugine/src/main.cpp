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
std::shared_ptr<Camera> createMainCamera(GLFWwindow* window);
std::shared_ptr<Shader> createBasicShader();
void updateMainCameraViewport(std::shared_ptr<Camera> mainCamera, GLFWwindow* window);

Vertex triangleVert[3] = { { glm::vec3( 0.0f,  1.0f, 0.0f), glm::vec2(0.0f, 1.0f) },
                           { glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec2(0.0f, 1.0f) },
                           { glm::vec3( 1.0f, -1.0f, 0.0f), glm::vec2(0.0f, 1.0f) }
						 };

Vertex cubeFrontVert[16] = {
    { glm::vec3(-0.5f,  0.5f, 0.5f), glm::vec2(0.0f, 1.0f) },
    { glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f) },
    { glm::vec3( 0.5f, -0.5f, 0.5f), glm::vec2(1.0f, 0.0f) },
    { glm::vec3( 0.5f,  0.5f, 0.5f), glm::vec2(1.0f, 1.0f) },

    { glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(1.0f, 1.0f) },
    { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 0.0f) },
    { glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f) },
    { glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec2(0.0f, 1.0f) },

    { glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec2(0.0f, 1.0f) },
    { glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f) },
    { glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 0.0f) },
    { glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec2(1.0f, 1.0f) },

    { glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(0.0f, 1.0f) },
    { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f) },
    { glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(1.0f, 0.0f) },
    { glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 1.0f) }
};
Vertex cubeTopVert[8] = {
    { glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(0.0f, 1.0f) },
    { glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec2(0.0f, 0.0f) },
    { glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 0.0f) },
    { glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec2(1.0f, 1.0f) },

    { glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 1.0f) },
    { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f) },
    { glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 0.0f) },
    { glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec2(1.0f, 1.0f) },
};

GLushort triangleIdx[6] = { 0, 1, 2 };
GLushort cubeFrontIdx[24] = {
     0,  1,  2,  0,  2,  3,
     7,  6,  5,  7,  5,  4,
     8,  9, 10,  8, 10, 11,
    12, 13, 14, 12, 14, 15
};
GLushort cubeTopIdx[12] = {
    0, 1, 2, 0, 2, 3,
    4, 5, 6, 4, 6, 7
};

GLint Z_POS[3] = {  0, -3, -6 };
GLint X_POS[3] = { -3,  0,  3 };

int main(int, char**) {
	GLFWwindow* window = initGLFW();

    if (!window) { return -1; }
    if (!init()) { return -1; }

    std::shared_ptr<Shader> shader = createBasicShader();
	if (!shader) { return -1; }

    State::defaultShader = shader;

    std::shared_ptr<Camera> mainCamera = createMainCamera(window);
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

	std::cout << "loading asian town... ";
	std::shared_ptr<Mesh> mesh = Mesh::load("data/asian_town.msh.xml");
    std::shared_ptr<Model> model = std::make_shared<Model>(mesh);
    model->setScale({ 10.0f, 10.0f, 10.0f });
	std::cout << "loaded" << std::endl;

    world->addEntity(model);

    return world;
}

std::shared_ptr<Camera> createMainCamera(GLFWwindow* window) {
	int screenWidth, screenHeight;
	glfwGetWindowSize(window, &screenWidth, &screenHeight);

    std::shared_ptr<Camera> mainCamera = std::make_shared<Camera>(glm::ivec2(screenWidth, screenHeight));
    mainCamera->setClearColor(glm::vec3(0.2f, 0.6f, 1.0f));
    mainCamera->setPosition(glm::vec3(0.0f, 0.1f, 0.0f));
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //glm::quat rot = glm::rotate(glm::quat(), glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    //mainCamera->setRotation(rot);


	mainCamera->setUpdateCB([mainCamera, window](float dt) {
        if (glfwGetKey(window, GLFW_KEY_W)) {
            mainCamera->move(glm::vec3(0.0f, 0.0f, -1.0f)*dt);
        }
        if (glfwGetKey(window, GLFW_KEY_A)) {
            mainCamera->move(glm::vec3(-1.0f, 0.0f, 0.0f)*dt);
        }
        if (glfwGetKey(window, GLFW_KEY_S)) {
            mainCamera->move(glm::vec3(0.0f, 0.0f, 1.0f)*dt);
        }
        if (glfwGetKey(window, GLFW_KEY_D)) {
            mainCamera->move(glm::vec3(1.0f, 0.0f, 0.0f)*dt);
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE)) {
            mainCamera->move(glm::vec3(0.0f, 1.0f, 0.0f)*dt);
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) || glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL)) {
            mainCamera->move(glm::vec3(0.0f, -1.0f, 0.0f)*dt);
        }

        glm::ivec4 halfVP = mainCamera->getViewport()/2;
        glm::vec2 viewportCenter = { halfVP.z, halfVP.w };


        double xpos = 0, ypos = 0;
        glfwGetCursorPos(window, &xpos, &ypos);
        glm::vec2 currMousePos(xpos, ypos );
        currMousePos = (viewportCenter - currMousePos) / viewportCenter;
        
        glm::quat xQuad = glm::rotate(glm::quat(), currMousePos.x, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::quat yQuad = glm::rotate(glm::quat(), currMousePos.y, glm::vec3(1.0f, 0.0f, 0.0f));
        mainCamera->setRotation(xQuad * yQuad);
	});

    return mainCamera;
}

void updateMainCameraViewport(std::shared_ptr<Camera> mainCamera, GLFWwindow* window) {
    int screenWidth, screenHeight;
    glfwGetWindowSize(window, &screenWidth, &screenHeight);
    mainCamera->setProjection(glm::perspective(glm::pi<float>()/3.0f, screenWidth / static_cast<float>(screenHeight), 0.001f, 100.0f));
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
        std::cout << Shader::getError() << std::endl;
        return nullptr;
    }
    return s;
}