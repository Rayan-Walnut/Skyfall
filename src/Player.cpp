#include "Player.h"

Player::Player()
: pos(0.0f, 1.0f, 0.0f), vel(0.0f), grounded(true)
{}

const glm::vec3 &Player::getPosition() const { return pos; }

void Player::update(float dt) {
    // simple gravity
    if (!grounded) {
        vel.y -= 9.81f * dt;
    }
    pos += vel * dt;
    if (pos.y <= 1.0f) { pos.y = 1.0f; vel.y = 0.0f; grounded = true; }
}

void Player::move(const glm::vec3 &delta) {
    pos += delta;
}

void Player::jump() {
    if (grounded) { vel.y = 5.0f; grounded = false; }
}
