#pragma once

#include <memory>
#include <vector>
#include "Camera.h"
#include "Entity.h"
#include "Light.h"

class World {
private:
    std::vector<std::shared_ptr<Entity>> _entityList;
    std::vector<std::shared_ptr<Camera>> _cameraList;
    std::vector<std::shared_ptr<Light>> _lightList;

    glm::vec3 _ambient;
public:
    const glm::vec3& getAmbient() const;
    void setAmbient(const glm::vec3& ambient);

	void addEntity(const std::shared_ptr<Entity>& entity);
	void removeEntity(const std::shared_ptr<Entity>& entity);
	size_t getNumEntities() const;

	const std::shared_ptr<Entity>& getEntity(size_t index) const;
	std::shared_ptr<Entity>& getEntity(size_t index);
	void update(float deltaTime);
	void draw();

};