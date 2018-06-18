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

    bool _shadowsEnabled;
    float _far;
    glm::mat4 _depthOrto;
    std::shared_ptr<Camera> _depthCamera;
    std::shared_ptr<Shader>	_depthShader;

    void castShadows();
    void drawEntities();

public:
    World();
    ~World();

    const glm::vec3& getAmbient() const;
    void setAmbient(const glm::vec3& ambient);

	void addEntity(const std::shared_ptr<Entity>& entity);
	void removeEntity(const std::shared_ptr<Entity>& entity);
	size_t getNumEntities() const;

    void setShadows(bool enable);
    void setDepthOrtho(float left, float right,
        float bottom, float top, float near, float far);

	const std::shared_ptr<Entity>& getEntity(size_t index) const;
	std::shared_ptr<Entity>& getEntity(size_t index);
	void update(float deltaTime);
	void draw();

};