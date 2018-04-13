#pragma once

#include <memory>
#include "Buffer.h"
#include "Material.h"
#include <vector>
#include <utility>

class Mesh {
protected:
	std::vector<std::pair<std::shared_ptr<Buffer>, Material>> _bufferList;

public:
    Mesh();

	void addBuffer( const std::shared_ptr<Buffer>& buffer,
					const Material& material);
	size_t getNumBuffers() const;

	const std::shared_ptr<Buffer>& getBuffer(size_t index) const;
	std::shared_ptr<Buffer>& getBuffer(size_t index);

    const Material& getMaterial(size_t index) const;
    Material& getMaterial(size_t index);

	void draw();

};