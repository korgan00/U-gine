#pragma once

#include <memory>
#include "Entity.h"
#include "Mesh.h"
#include <functional>

class Model : public Entity {
protected:
	std::shared_ptr<Mesh> _mesh;

    bool _animate;
    int _fps;
    float _currentFrame;
    std::shared_ptr<std::vector<glm::mat4>> _animMatrices;
public:
	Model(const std::shared_ptr<Mesh>& mesh);
    virtual void draw() override;
    virtual void update(float deltaTime) override;

    void animate(bool animate);
    int getFps() const;
    void setFps(int fps);
    float getCurrentFrame() const;
    void setCurrentFrame(float frame);
    void calculateAnimMatrices(float frame, const std::vector<Bone>& bones);

};