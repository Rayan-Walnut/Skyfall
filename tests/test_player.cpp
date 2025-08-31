#include <cassert>
#include "../src/Player.h"

int main() {
    Player p;
    float dt = 0.1f;
    p.update(dt);
    // should still be grounded at initial pos
    auto pos = p.getPosition();
    assert(pos.y == 1.0f);
    p.jump();
    p.update(dt);
    pos = p.getPosition();
    // after a small dt, y should be > 1
    assert(pos.y > 1.0f);
    return 0;
}
