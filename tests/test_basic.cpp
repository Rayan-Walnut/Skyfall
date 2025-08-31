#include <iostream>
#include <cassert>
#include <glm/glm.hpp>

int main() {
    glm::vec3 a(1.0f, 2.0f, 3.0f);
    glm::vec3 b(2.0f, 3.0f, 4.0f);
    glm::vec3 c = a + b;
    assert(c.x == 3.0f && c.y == 5.0f && c.z == 7.0f);
    std::cout << "basic tests passed\n";
    return 0;
}
