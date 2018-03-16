#pragma once

#include <memory>
#include "Entity.h"
#include "Mesh.h"

class Model : Entity {
protected:
	std::shared_ptr<Mesh> _mesh;
public:
	Model(const std::shared_ptr<Mesh>& mesh);
	virtual void draw() override;
};