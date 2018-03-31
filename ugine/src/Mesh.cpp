

#include "Mesh.h"
#include "State.h"

using namespace std;

Mesh::Mesh() {
}


void Mesh::addBuffer(const shared_ptr<Buffer>& buffer,
					 const shared_ptr<Shader>& shader) {
	_bufferList.push_back(pair<std::shared_ptr<Buffer>, std::shared_ptr<Shader>>(buffer, shader != nullptr ? shader : State::defaultShader));
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
void Mesh::draw() {
	size_t amount = _bufferList.size();
    glm::mat4 mvp = State::projectionMatrix * State::viewMatrix * State::modelMatrix;

	for (size_t i = 0; i < amount; ++i) {
        _bufferList[i].second->use();
        //GLint locMVP = _bufferList[i].second->getLocation("mvp");
        _bufferList[i].second->setMatrix(_bufferList[i].second->getLocation("mvp"), mvp);
		_bufferList[i].first->draw(*_bufferList[i].second);
	}
}
