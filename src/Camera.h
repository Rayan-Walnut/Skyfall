#pragma once

#include <glm/glm.hpp>

class Camera {
public:
    Camera();
    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix(float aspect) const;

    void setTarget(const glm::vec3 &t);
    void update(float dt);

    void setYawPitch(float yaw, float pitch);
    void addYawPitch(float dyaw, float dpitch);

private:
    glm::vec3 target;
    float distance;
    float yaw, pitch;
};
