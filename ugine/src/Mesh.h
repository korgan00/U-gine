#pragma once

#include <memory>
#include "Buffer.h"
#include "Bone.h"
#include "Material.h"
#include <vector>
#include <utility>

class Mesh {
protected:
	std::vector<std::pair<std::shared_ptr<Buffer>, Material>> _bufferList;

    std::vector<Bone> _boneList;
    GLushort _lastFrame;
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

	static std::shared_ptr<Mesh> load(const char* filename, const std::shared_ptr<Shader>& shader = nullptr);
    
    void addBone(const Bone& bone);
    const std::vector<Bone>& getBones() const;
    GLushort getLastFrame() const;
    void setLastFrame(GLushort lastFrame);
    int getBoneIndex(const char* name) const;

};