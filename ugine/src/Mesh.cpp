

#include "Mesh.h"
#include "State.h"

using namespace std;

Mesh::Mesh() {
}


void Mesh::addBuffer(const shared_ptr<Buffer>& buffer,
					 const shared_ptr<Shader>& shader = nullptr) {
	bufferList.push_back(pair<std::shared_ptr<Buffer>, std::shared_ptr<Shader>>(buffer, shader != nullptr ? shader : State::defaultShader));
}


size_t Mesh::getNumBuffers() const {
	return bufferList.size();
}
const shared_ptr<Buffer>& Mesh::getBuffer(size_t index) const {
	return bufferList[index].first;
}
shared_ptr<Buffer>& Mesh::getBuffer(size_t index) {
	return bufferList[index].first;
}
void Mesh::draw() {
	GLuint amount = bufferList.size();
	for (int i = 0; i < amount; ++i) {
		bufferList[i].first->draw(*bufferList[i].second);
	}
}
