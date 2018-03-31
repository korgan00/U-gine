#pragma once

#include <memory>
#include <vector>
#include "Camera.h"
#include "Entity.h"

class World {
private:
    std::vector<std::shared_ptr<Entity>> entityList;
    std::vector<std::shared_ptr<Camera>> cameraList;

public:
	void addEntity(const std::shared_ptr<Entity>& entity);
	void removeEntity(const std::shared_ptr<Entity>& entity);
	size_t getNumEntities() const;

	const std::shared_ptr<Entity>& getEntity(size_t index) const;
	std::shared_ptr<Entity>& getEntity(size_t index);
	void update(float deltaTime);
	void draw();

};