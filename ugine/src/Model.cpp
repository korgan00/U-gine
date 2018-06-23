#include "Model.h"
#include "common.h"
#include "State.h"


Model::Model(const std::shared_ptr<Mesh>& mesh) : _mesh(mesh), _animate(false), _fps(16), _currentFrame(0.0f) {
    _animMatrices = std::make_shared<std::vector<glm::mat4>>();
}

void Model::draw() {
#define SCALE(m) glm::scale(m, _scale)
#define ROTATE(m) glm::rotate(m, glm::angle(_rotation), glm::axis(_rotation))
#define TRANSLATE(m) glm::translate(m, _position)

	State::modelMatrix = SCALE(ROTATE(TRANSLATE(glm::mat4())));
    State::animation = _animate;
    State::animMatrices = _animMatrices;

    _mesh->draw();
}
void Model::update(float deltaTime) {
    Entity::update(deltaTime);
    if (_animate) {
        _currentFrame += _fps * deltaTime;

        while (_currentFrame > _mesh->getLastFrame()) {
            _currentFrame -= _mesh->getLastFrame();
        }
        while (_currentFrame < 0.0f) {
            _currentFrame += _mesh->getLastFrame();
        }
        calculateAnimMatrices(_currentFrame, _mesh->getBones());
    }
}


void Model::animate(bool animate) {
    _animate = animate;
}
int Model::getFps() const {
    return _fps;
}
void Model::setFps(int fps) {
    _fps = fps;
}
float Model::getCurrentFrame() const {
    return _currentFrame;
}
void Model::setCurrentFrame(float frame) {
    _currentFrame = frame;
}
void Model::calculateAnimMatrices(float frame, const std::vector<Bone>& bones) {
    _animMatrices->clear();
    for (Bone bone : bones) {
        int parentIdx = bone.getParentIndex();
        if (parentIdx == -1) {
            _animMatrices->push_back(bone.calculateAnimMatrix(frame));
        } else {
            _animMatrices->push_back(_animMatrices->at(parentIdx) * bone.calculateAnimMatrix(frame));
        }
    }
    
    for (int i = 0; i < _animMatrices->size(); ++i) {
        (*_animMatrices)[i] = _animMatrices->at(i) * bones[i].getInvPoseMatrix();
    }
}