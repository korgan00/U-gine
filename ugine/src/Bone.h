#pragma once

#include "common.h"
#include <vector>

class Bone {
private:
    const char* _name;
    int _parentIdx;

    typedef std::pair<int, glm::vec3> framevec3;
    typedef std::pair<int, glm::quat> framequat;

    glm::mat4 _invPoseMatrix;
    std::vector<framevec3> _framePositions;
    std::vector<framequat> _frameRotations;
    std::vector<framevec3> _frameScales;

    glm::vec3 calculateMix(const std::vector<framevec3> &_frameVec, float frame) const;

public:

    Bone(const char* name, int parentIndex);

    const char* getName() const;
    int getParentIndex() const;

    const glm::mat4& getInvPoseMatrix() const;
    void setInvPoseMatrix(const glm::mat4& m);

    void addPosition(uint16_t frame, const glm::vec3& position);
    void addRotation(uint16_t frame, const glm::quat& rotation);
    void addScale(uint16_t frame, const glm::vec3& scale);

    glm::mat4 calculateAnimMatrix(float frame) const;
    glm::vec3 calculatePosition(float frame) const;
    glm::quat calculateRotation(float frame) const;
    glm::vec3 calculateScale(float frame) const;

};