
#include "Model.h"
#include "common.h"
#include "State.h"



Model::Model(const std::shared_ptr<Mesh>& mesh) : _mesh(mesh) {}
void Model::draw() {
	State::modelMatrix = glm::scale(glm::rotate(glm::translate(glm::mat4(), _position), glm::angle(_rotation), glm::axis(_rotation)), _scale);
}