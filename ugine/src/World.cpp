
#include "World.h"
#include "State.h"
#include <memory>
using namespace std;

typedef vector<shared_ptr<Entity>>::iterator entityIt;
typedef vector<shared_ptr<Camera>>::iterator cameraIt;
typedef vector<shared_ptr<Light>>::iterator lightIt;


std::shared_ptr<Shader> createDepthShader() {
    // read shaders
    std::string vertexShader = readString("data/shadowsShader.vert");
    std::string fragmentShader = readString("data/shadowsShader.frag");
    if (vertexShader == "" || fragmentShader == "") {
        return nullptr;
    }

    const char* vsCode = vertexShader.c_str();
    const char* fsCode = fragmentShader.c_str();
    std::shared_ptr<Shader> s = Shader::createShader(vsCode, fsCode);
    if (Shader::getError() != "") {
        return nullptr;
    }
    return s;
}

World::World() {
    std::shared_ptr<Texture> depthTex = make_shared<Texture>(glm::ivec2(1024, 1024), true);
    std::shared_ptr<Framebuffer> frameBuffer = make_shared<Framebuffer>(nullptr, depthTex);
    _depthCamera = make_shared<Camera>(depthTex->getSize());
    _depthCamera->setFramebuffer(frameBuffer);
    _depthShader = createDepthShader();
}
World::~World() {}


const glm::vec3& World::getAmbient() const {
    return _ambient;
}
void World::setAmbient(const glm::vec3& ambient) {
    _ambient = ambient;
}

void World::addEntity(const shared_ptr<Entity>& entity) {
    shared_ptr<Camera> camera = dynamic_pointer_cast<Camera>(entity);
    if (camera) {
        _cameraList.push_back(camera);
    }

    shared_ptr<Light> light = dynamic_pointer_cast<Light>(entity);
    if (light) {
        _lightList.push_back(light);
    }

	_entityList.push_back(entity);
}

void World::removeEntity(const shared_ptr<Entity>& entity) {
    entityIt it = find(_entityList.begin(), _entityList.end(), entity);
    if (it != _entityList.end()) {
        _entityList.erase(it);
    }


    shared_ptr<Camera> camera = dynamic_pointer_cast<Camera>(entity);
    if (camera) {
		cameraIt camIt = find(_cameraList.begin(), _cameraList.end(), camera);
        if (camIt != _cameraList.end()) {
            _cameraList.erase(camIt);
        }
    }

    shared_ptr<Light> light = dynamic_pointer_cast<Light>(entity);
    if (light) {
        lightIt lIt = find(_lightList.begin(), _lightList.end(), light);
        if (lIt != _lightList.end()) {
            _lightList.erase(lIt);
        }
    }
}

size_t World::getNumEntities() const {
	return _entityList.size();
}

void World::setShadows(bool enable) {
    State::shadows = enable;
}
void World::setDepthOrtho(float left, float right,
                   float bottom, float top, 
                   float near, float far) {
    _depthOrto = glm::ortho(left, right, bottom, top, near, far);
    _far = far;
}

const shared_ptr<Entity>& World::getEntity(size_t index) const {
	return _entityList.at(index);
}
shared_ptr<Entity>& World::getEntity(size_t index) {
    return _entityList.at(index);
}
void World::update(float deltaTime) {
    entityIt it = _entityList.begin();

    while (it != _entityList.end()) {
        (*it)->update(deltaTime);
        it++;
    }
}
std::shared_ptr<Light> firstDirectionalLight(std::vector<std::shared_ptr<Light>> lightList) {
    lightIt lIt = lightList.begin();
    while (lIt != lightList.end()) {
        if ((*lIt)->getType() == Type::DIRECTIONAL) {
            return *lIt;
        }
        lIt++;
    }
    return nullptr;
}

void World::drawEntities() {
    entityIt eIt = _entityList.begin();
    while (eIt != _entityList.end()) {
        (*eIt)->draw();
        eIt++;
    }
}

void World::castShadows() {
    std::shared_ptr<Light> shadowCasterLight = firstDirectionalLight(_lightList);
    if (!shadowCasterLight) {
        return;
    }

    State::overrideShader = _depthShader;

    glm::vec3 lightPosNorm = glm::normalize(shadowCasterLight->getPosition());
    glm::vec3 camPosition = lightPosNorm * _far;
    _depthCamera->setPosition(camPosition);

    glm::vec3 lightRayDir = -lightPosNorm;
    float pitch = glm::asin(lightRayDir.y);
    float yaw = atan2(-lightRayDir.x, -lightRayDir.z);
    float roll = 0;
    glm::quat rotation = glm::rotate(glm::quat(), yaw, glm::vec3(0.0f, 1.0f, 0.0f));
    rotation = glm::rotate(glm::quat(), pitch, glm::vec3(1.0f, 0.0f, 0.0f));

    _depthCamera->prepare();
    drawEntities();

    const glm::mat4 bias(glm::vec4(0.5f, 0, 0, 0.5f), glm::vec4(0, 0.5f, 0, 0.5f), glm::vec4(0, 0, 0.5f, 0.5f), glm::vec4(0, 0, 0, 1.0f));
    State::depthBiasMatrix = bias * State::projectionMatrix * State::viewMatrix;

    _depthCamera->getFramebuffer()->getDepthTexture()->bind(SHADOWMAP_LAYER);

    State::overrideShader = nullptr;
}

void World::draw() {
    // Pre pass
    if (_shadowsEnabled) {
        castShadows();
    }

    // Pass
    cameraIt cIt = _cameraList.begin();

    State::lights = _lightList;
    State::ambient = _ambient;

    while (cIt != _cameraList.end()) {
        (*cIt)->prepare();
        drawEntities();
        cIt++;
    }
}