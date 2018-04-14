#include "Model.h"
#include "common.h"
#include "State.h"


Model::Model(const std::shared_ptr<Mesh>& mesh) : _mesh(mesh) {}

void Model::draw() {
#define SCALE(m) glm::scale(m, _scale)
#define ROTATE(m) glm::rotate(m, glm::angle(_rotation), glm::axis(_rotation))
#define TRANSLATE(m) glm::translate(m, _position)

	State::modelMatrix = SCALE(ROTATE(TRANSLATE(glm::mat4())));
    _mesh->draw();
}