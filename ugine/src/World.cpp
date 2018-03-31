
#include "World.h"
#include <memory>
using namespace std;

typedef vector<shared_ptr<Entity>>::iterator entityIt;
typedef vector<shared_ptr<Camera>>::iterator cameraIt;

void World::addEntity(const shared_ptr<Entity>& entity) {
    shared_ptr<Camera> camera = dynamic_pointer_cast<Camera>(entity);
    if (camera) {
        cameraList.push_back(camera);
    }
	entityList.push_back(entity);
}

void World::removeEntity(const shared_ptr<Entity>& entity) {
    entityIt it = entityList.begin();

    while (*it != entity && it != entityList.end()) it++;

    if (it != entityList.end()) {
        entityList.erase(it);
    }


    shared_ptr<Camera> camera = dynamic_pointer_cast<Camera>(entity);
    if (camera) {
        cameraIt it = cameraList.begin();

        while (*it != entity && it != cameraList.end()) it++;

        if (it != cameraList.end()) {
            cameraList.erase(it);
        }
    }
}

size_t World::getNumEntities() const {
	return entityList.size();
}

const shared_ptr<Entity>& World::getEntity(size_t index) const {
	return entityList.at(index);
}
shared_ptr<Entity>& World::getEntity(size_t index) {
    return entityList.at(index);
}
void World::update(float deltaTime) {
    entityIt it = entityList.begin();

    while (it != entityList.end()) {
        (*it)->update(deltaTime);
        it++;
    }
}
void World::draw() {
    cameraIt cIt = cameraList.begin();

    while (cIt != cameraList.end()) {
        (*cIt)->prepare();

        entityIt eIt = entityList.begin();
        while (eIt != entityList.end()) {
            (*eIt)->draw();
            eIt++;
        }
        cIt++;
    }
}