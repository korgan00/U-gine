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
#include "Emitter.h"
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
    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
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

Material particleMaterial(const char* name, BlendMode bm) {
	Material m(Texture::load(name));
	m.setBlendMode(bm);
	m.setCulling(false);
	m.setDepthWrite(false);
	m.setLighting(false);

	return m;
}

std::shared_ptr<World> createWorld(std::shared_ptr<Camera> mainCamera) {
    std::shared_ptr<World> world = std::make_shared<World>();

    world->addEntity(mainCamera);

    // MODEL
    std::cout << "loading scene... ";
    std::shared_ptr<Mesh> mesh = Mesh::load("data/scene.msh.xml");
    std::shared_ptr<Model> model = std::make_shared<Model>(mesh);
    
    model->setScale(glm::vec3(1.0f, 1.0f, 1.0f));
    model->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    std::cout << "loaded" << std::endl;

    world->addEntity(model);

    // LIGHTING
    std::shared_ptr<Light> lightSun = std::make_shared<Light>();
    lightSun->setType(Type::DIRECTIONAL);
    std::shared_ptr<float> angle = std::make_shared<float>(90.0f);

    lightSun->setUpdateCB([lightSun, angle](float dt) {
        *angle += dt * 64.0f;
        glm::quat rot = glm::rotate(glm::quat(), glm::radians(*angle), glm::vec3(0.0f, 1.0f, 0.0f));
        rot = glm::rotate(rot, glm::radians(-30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        lightSun->setRotation(rot);
        lightSun->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
        lightSun->move(glm::vec3(0.0f, 0.0f, 1.0f));
    });

    world->addEntity(lightSun);

    // WORLD CONFIG
    world->setAmbient({ 0.2f, 0.2f, 0.2f });
    world->setDepthOrtho(-10, 10, -10, 10, 1, 100);
    world->setShadows(true);

    return world;
}

std::shared_ptr<Camera> createMainCamera(GLFWwindow* window) {
	int screenWidth, screenHeight;
	glfwGetWindowSize(window, &screenWidth, &screenHeight);

    std::shared_ptr<Camera> mainCamera = std::make_shared<Camera>(glm::ivec2(screenWidth, screenHeight));
    mainCamera->setClearColor(glm::vec3(0.2f, 0.4f, 0.8f));
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glm::quat rot = glm::rotate(glm::quat(), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rot = glm::rotate(rot, glm::radians(-30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    mainCamera->setRotation(rot);
    mainCamera->setPosition(glm::vec3(0.0f, 8.0f, -12.0f));
	
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