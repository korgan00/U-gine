#pragma once

#include "common.h"

typedef struct{
    glm::vec3 position;
    glm::vec2 texCoords;
    glm::vec3 normal;
    glm::vec3 tangent;
    glm::vec4 boneIndices;
    glm::vec4 boneWeights;
} Vertex;
