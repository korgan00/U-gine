#include "Mesh.h"
#include "State.h"

using namespace std;

Mesh::Mesh() {
}


void Mesh::addBuffer(const shared_ptr<Buffer>& buffer,
					 const Material& material) {
	_bufferList.push_back(pair<std::shared_ptr<Buffer>, Material>(buffer, material));
}


size_t Mesh::getNumBuffers() const {
	return _bufferList.size();
}
const shared_ptr<Buffer>& Mesh::getBuffer(size_t index) const {
	return _bufferList[index].first;
}
shared_ptr<Buffer>& Mesh::getBuffer(size_t index) {
	return _bufferList[index].first;
}
const Material& Mesh::getMaterial(size_t index) const {
    return _bufferList[index].second;
}
Material& Mesh::getMaterial(size_t index) {
    return _bufferList[index].second;
}

void Mesh::draw() {
	size_t amount = _bufferList.size();
    //glm::mat4 mvp = State::projectionMatrix * State::viewMatrix * State::modelMatrix;

	for (size_t i = 0; i < amount; ++i) {
        _bufferList[i].second.prepare();
		_bufferList[i].first->draw(*(_bufferList[i].second.getShader()));
	}
}
