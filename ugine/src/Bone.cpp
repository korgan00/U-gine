
#include "Bone.h"
#include <algorithm>
#include <functional>

Bone::Bone(const char* name, int parentIndex) : _name(name), _parentIdx(parentIndex) {

}

const char* Bone::getName() const {
    return _name;
}
int Bone::getParentIndex() const {
    return _parentIdx;
}

const glm::mat4& Bone::getInvPoseMatrix() const {
    return _invPoseMatrix;
}
void Bone::setInvPoseMatrix(const glm::mat4& m) {
    _invPoseMatrix = m;
}

void Bone::addPosition(uint16_t frame, const glm::vec3& position) {
    _framePositions.push_back(framevec3(frame, position));
    /*
    std::sort(_framePositions.begin(), _framePositions.end(), [](framevec3 a, framevec3 b) {
        return a.first < b.first;
    });
    */
}
void Bone::addRotation(uint16_t frame, const glm::quat& rotation) {
    _frameRotations.push_back(framequat(frame, rotation));
    /*
    std::sort(_frameRotations.begin(), _frameRotations.end(), [](framequat a, framequat b) {
        return a.first < b.first;
    });
    */
}
void Bone::addScale(uint16_t frame, const glm::vec3& scale) {
    _frameScales.push_back(framevec3(frame, scale));
    /*
    std::sort(_frameScales.begin(), _frameScales.end(), [](framevec3 a, framevec3 b) {
        return a.first < b.first;
    });
    */
}

glm::mat4 Bone::calculateAnimMatrix(float frame) const {
#define SCALE(m) glm::scale(m, calculateScale(frame))
#define ROTATE(m) glm::rotate(m, glm::angle(rot), glm::axis(rot))
#define TRANSLATE(m) glm::translate(m, calculatePosition(frame))

    glm::quat rot = calculateRotation(frame);
    return SCALE(ROTATE(TRANSLATE(glm::mat4())));
}

glm::vec3 Bone::calculateMix(const std::vector<framevec3> &_frameVec, float frame) const {
    for (int i = 0; i < _frameVec.size(); ++i) {
        if (_frameVec[i].first == frame) {
            return _frameVec[i].second;
        } else if (_frameVec[i].first >= frame) {
            const framevec3& a = _frameVec[i - 1];
            const framevec3& b = _frameVec[i];
            float mixAmount = (frame - a.first) / (b.first - a.first);
            return glm::mix(a.second, b.second, mixAmount);
        }
    }
    if (_frameVec.size() > 0) {
        return _frameVec.back().second;
    }
    return glm::vec3();

}

glm::vec3 Bone::calculatePosition(float frame) const {
    return calculateMix(_framePositions, frame);
}
glm::quat Bone::calculateRotation(float frame) const {
    for (int i = 0; i < _frameRotations.size(); ++i) {
        if (_frameRotations[i].first == frame) {
            return _frameRotations[i].second;
        } else if (_frameRotations[i].first >= frame) {
            const framequat& a = _frameRotations[i - 1];
            const framequat& b = _frameRotations[i];
            float mixAmount = (frame - a.first) / (b.first - a.first);
            return glm::slerp(a.second, b.second, mixAmount);
        }
    }
    if (_frameRotations.size() > 0) {
        return _frameRotations.back().second;
    }
    return glm::quat();
}
glm::vec3 Bone::calculateScale(float frame) const {
    return calculateMix(_frameScales, frame);
}