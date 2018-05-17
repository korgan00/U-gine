#pragma once

#include "Entity.h"
#include "Material.h"
#include "Buffer.h"

class Billboard : public Entity {
private:
	Material _material;
	glm::vec2 _size;
	float _spin;

	std::shared_ptr<Buffer> _buffer;

public:
	Billboard(const Material& mat);

	const Material& getMaterial() const;
	Material& getMaterial();

	const glm::vec2& getSize() const;
	void setSize(const glm::vec2& size);

	float getSpin() const;
	void setSpin(float spin);

	virtual void draw() override;
};