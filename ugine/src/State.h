#pragma once

#include "common.h"
#include "Shader.h"
#include "Light.h"
#include <memory>
#include <vector>

class State {
protected:
	State();
	~State();
public:
	static std::shared_ptr<Shader>	defaultShader;
	static glm::mat4 				projectionMatrix;
	static glm::mat4 				viewMatrix;
	static glm::mat4 				modelMatrix;


    static std::vector<std::shared_ptr<Light>>	lights;
    static glm::vec3						    ambient;
};