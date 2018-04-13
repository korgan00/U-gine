#pragma once

#include <memory>
#include "Entity.h"
#include "Mesh.h"
#include <functional>

class Model : public Entity {
protected:
	std::shared_ptr<Mesh> _mesh;
	std::function<void(float)> _updateCB;
public:
	Model(const std::shared_ptr<Mesh>& mesh);
    virtual void draw() override;
    virtual void update(float deltaTime) override;
	void		 setUpdateCB(const std::function<void(float)> updateFunc);
};