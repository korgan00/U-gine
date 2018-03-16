#pragma once

#include <memory>
#include <list>
#include "Entity.h"

class World {
private:
	std::list<std::shared_ptr<Entity>> entityList;
	

public:
	void addEntity(const std::shared_ptr<Entity>& entity);
	void removeEntity(const std::shared_ptr<Entity>& entity);
	size_t getNumEntities() const;

	const std::shared_ptr<Entity>& getEntity(size_t index) const;
	std::shared_ptr<Entity>& getEntity(size_t index);
	void update(float deltaTime);
	void draw();

};