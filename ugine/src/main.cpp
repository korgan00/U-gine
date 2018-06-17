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
    std::cout << "loading skybox... ";
    std::shared_ptr<Mesh> meshSkybox = Mesh::load("data/skybox.msh.xml");
    std::shared_ptr<Model> modelSkybox = std::make_shared<Model>(meshSkybox);
    for (int i = 0; i < meshSkybox->getNumBuffers(); i++) {
        meshSkybox->getMaterial(i).setLighting(false);
    }
    modelSkybox->setScale(glm::vec3(1.0f, 1.0f, 1.0f));
    std::cout << "loaded" << std::endl;

    world->addEntity(modelSkybox);

    modelSkybox->setUpdateCB([mainCamera, modelSkybox](float dt) {
        modelSkybox->setPosition(mainCamera->getPosition());
    });

    std::cout << "loading suzanne_refract... ";
    std::shared_ptr<Mesh> meshSuzanne = Mesh::load("data/suzanne_refract.msh.xml");
    std::shared_ptr<Model> modelSuzanne = std::make_shared<Model>(meshSuzanne);
    modelSuzanne->setScale(glm::vec3(1.0f, 1.0f, 1.0f));
    modelSuzanne->setPosition(glm::vec3(-2.0f, 0.0f, 2.0f));
    std::cout << "loaded" << std::endl;

    world->addEntity(modelSuzanne);

    std::cout << "loading teapot_reflect... ";
    std::shared_ptr<Mesh> meshTeapot = Mesh::load("data/teapot_reflect.msh.xml");
    std::shared_ptr<Model> modelTeapot = std::make_shared<Model>(meshTeapot);
    modelTeapot->setScale(glm::vec3(1.0f, 1.0f, 1.0f));
    modelTeapot->setPosition(glm::vec3(2.0f, 0.0f, 2.0f));
    std::cout << "loaded" << std::endl;

    world->addEntity(modelTeapot);
    
    std::cout << "loading cube... ";
    std::shared_ptr<Mesh> meshCube = Mesh::load("data/cube.msh.xml");
    std::shared_ptr<Model> modelCube = std::make_shared<Model>(meshCube);
    modelCube->setScale(glm::vec3(1.0f, 1.0f, 1.0f));
    modelCube->setPosition(glm::vec3(-2.0f, 0.0f, -2.0f));
    std::cout << (meshCube? "loaded" : "error") << std::endl;

    world->addEntity(modelCube);

    // LIGHTING
    world->setAmbient({ 0.1f, 0.1f, 0.1f });

    std::shared_ptr<Light> lightStatic = std::make_shared<Light>();
    lightStatic->setType(Type::POINT);

    world->addEntity(lightStatic);

    lightStatic->setUpdateCB([mainCamera, lightStatic](float dt) {
        lightStatic->setPosition(mainCamera->getPosition());
    });

    return world;
}

/*
std::shared_ptr<World> createWorld(std::shared_ptr<Camera> mainCamera) {
    std::shared_ptr<World> world = std::make_shared<World>();
    world->addEntity(mainCamera);

	// MODEL
    std::cout << "loading column... ";
    std::shared_ptr<Mesh> mesh = Mesh::load("data/column.msh.xml");
    std::shared_ptr<Model> model = std::make_shared<Model>(mesh);
	model->setScale(glm::vec3(0.01f, 0.01f, 0.01f));
    std::cout << "loaded" << std::endl;

    world->addEntity(model);

	// PARTICLES
	std::shared_ptr<Emitter> smokeEmitter = std::make_shared<Emitter>(particleMaterial("data/smoke.png", BlendMode::ALPHA), true);
	smokeEmitter->setPosition(glm::vec3(0.0f, 7.0f, 0.0f));
	smokeEmitter->setRateRange(5.0f, 10.0f);
	smokeEmitter->setLifetimeRange(1.0f, 5.0f);
	smokeEmitter->setVelocityRange(glm::vec3(-0.1f, 1.0f, -0.1f), glm::vec3(0.1f, 4.0f, 0.1f));
	smokeEmitter->setSpinVelocityRange(glm::radians(30.0f), glm::radians(60.0f));
    smokeEmitter->setScaleRange(0.05f, 0.1f);
    smokeEmitter->setColorRange(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	smokeEmitter->emit(true);

	std::shared_ptr<Emitter> flameEmitter = std::make_shared<Emitter>(particleMaterial("data/flame.png", BlendMode::ADD), false);
	flameEmitter->setPosition(glm::vec3(0.0f, 6.5f, 0.0f));
	flameEmitter->setRateRange(10.0f, 25.0f);
	flameEmitter->setLifetimeRange(0.5f, 0.5f);
	flameEmitter->setVelocityRange(glm::vec3(-1.0f, 5.0f, -1.0f), glm::vec3(1.0f, 10.0f, 1.0f));
	flameEmitter->setSpinVelocityRange(glm::radians(0.0f), glm::radians(0.0f));
	flameEmitter->setScaleRange(0.025f, 0.1f);
    flameEmitter->setColorRange(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	flameEmitter->emit(true);

	world->addEntity(smokeEmitter);
	world->addEntity(flameEmitter);

	// LIGHTING
    world->setAmbient({ 0.1f, 0.1f, 0.1f });

    std::shared_ptr<Light> lightStatic = std::make_shared<Light>();
	lightStatic->setType(Type::POINT);
    lightStatic->setPosition(glm::vec3(0.0f, 7.0f, -1.0f));
    
    world->addEntity(lightStatic);

    return world;
}
*/
std::shared_ptr<Camera> createMainCamera(GLFWwindow* window) {
	int screenWidth, screenHeight;
	glfwGetWindowSize(window, &screenWidth, &screenHeight);

    std::shared_ptr<Camera> mainCamera = std::make_shared<Camera>(glm::ivec2(screenWidth, screenHeight));
    mainCamera->setClearColor(glm::vec3(0.2f, 0.4f, 0.8f));
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
    std::shared_ptr<float> accumDt = std::make_shared<float>(0.0f);

    mainCamera->setUpdateCB([mainCamera, window, accumDt](float dt) {
        *accumDt += dt;
        glm::quat rot = glm::rotate(glm::quat(), glm::radians(*accumDt * 25), glm::vec3(0.0f, 1.0f, 0.0f));
        rot = glm::rotate(rot, glm::radians(-20.0f * sinf(*accumDt)), glm::vec3(1.0f, 0.0f, 0.0f));
        mainCamera->setRotation(rot);
        mainCamera->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
        mainCamera->move(glm::vec3(0.0f, 0.0f, 10.0f));
    });

    /*
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
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL)) {
            mainCamera->move(glm::vec3(0.0f, -1.0f, 0.0f)*dt);
        }

        glm::ivec4 halfVP = mainCamera->getViewport()/2;
        glm::vec2 viewportCenter = { halfVP.z, halfVP.w };

        double xpos = 0, ypos = 0;
        glfwGetCursorPos(window, &xpos, &ypos);
        glm::vec2 currMousePos(xpos, ypos);
        currMousePos = (viewportCenter - currMousePos) / viewportCenter;
        
        glm::quat xQuad = glm::rotate(glm::quat(), currMousePos.x, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::quat yQuad = glm::rotate(glm::quat(), currMousePos.y, glm::vec3(1.0f, 0.0f, 0.0f));
        //mainCamera->setRotation(xQuad * yQuad);
	});
	*/
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