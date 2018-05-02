
#include "World.h"
#include "State.h"
#include <memory>
using namespace std;

typedef vector<shared_ptr<Entity>>::iterator entityIt;
typedef vector<shared_ptr<Camera>>::iterator cameraIt;
typedef vector<shared_ptr<Light>>::iterator lightIt;

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
void World::draw() {
    cameraIt cIt = _cameraList.begin();

    State::lights = _lightList;
    State::ambient = _ambient;

    while (cIt != _cameraList.end()) {
        (*cIt)->prepare();

        entityIt eIt = _entityList.begin();
        while (eIt != _entityList.end()) {
            (*eIt)->draw();
            eIt++;
        }
        cIt++;
    }
}