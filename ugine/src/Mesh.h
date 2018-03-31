#pragma once

#include <memory>
#include "Buffer.h"
#include <vector>

class Mesh {
protected:

	std::vector<std::pair<std::shared_ptr<Buffer>, std::shared_ptr<Shader>>> _bufferList;

public:

    Mesh();

	void addBuffer( const std::shared_ptr<Buffer>& buffer,
					const std::shared_ptr<Shader>& shader = nullptr);
	size_t getNumBuffers() const;
	const std::shared_ptr<Buffer>& getBuffer(size_t index) const;
	std::shared_ptr<Buffer>& getBuffer(size_t index);
	void draw();



};