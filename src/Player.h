#pragma once
#include <glm/glm.hpp>

class Player {
public:
    Player();
    const glm::vec3 &getPosition() const;
    void update(float dt);
    void move(const glm::vec3 &delta);
    void jump();

private:
    glm::vec3 pos;
    glm::vec3 vel;
    bool grounded;
};
