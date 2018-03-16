
#include "World.h"


void World::addEntity(const std::shared_ptr<Entity>& entity) {
	entityList.push_back(entity);
}
void World::removeEntity(const std::shared_ptr<Entity>& entity) {
	entityList.remove(entity);
}
size_t World::getNumEntities() const {
	return entityList.size();
}

const std::shared_ptr<Entity>& World::getEntity(size_t index) const {
	entityList;
}
std::shared_ptr<Entity>& World::getEntity(size_t index) {

}
void World::update(float deltaTime) {

}
void World::draw() {

}