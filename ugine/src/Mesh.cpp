

#include "Mesh.h"
#include "State.h"

using namespace std;

Mesh::Mesh() {
}


void Mesh::addBuffer(const shared_ptr<Buffer>& buffer,
					 const shared_ptr<Shader>& shader = nullptr) {
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
	GLuint amount = _bufferList.size();
	for (int i = 0; i < amount; ++i) {
		_bufferList[i].first->draw(*_bufferList[i].second);
	}
}
