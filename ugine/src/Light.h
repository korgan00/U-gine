#pragma once

#include "Entity.h"
#include "Shader.h"

enum Type {
    DIRECTIONAL = 0,
    POINT = 1
};

class Light : public Entity {
private:

    Type _type;
    glm::vec3 _color;
    float _linearAttenuation;

public:

    Light();
    ~Light();

    Type			getType() const;
    void			setType(Type type);
    const glm::vec3&	getColor() const;
    void			setColor(const glm::vec3& color);
    float			getLinearAttenuation() const;
    void			setLinearAttenuation(float att);
    void			prepare(int index, std::shared_ptr<Shader>& shader) const;
};