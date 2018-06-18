#include "State.h"

std::shared_ptr<Shader>	            State::defaultShader = nullptr;
glm::mat4 				            State::projectionMatrix;
glm::mat4 				            State::viewMatrix;
glm::mat4 				            State::modelMatrix;


std::shared_ptr<Shader>	            State::overrideShader = nullptr;
glm::mat4				            State::depthBiasMatrix;
bool					            State::shadows = false;

std::vector<std::shared_ptr<Light>>	State::lights;
glm::vec3						    State::ambient;
glm::vec3						    State::eyePos;