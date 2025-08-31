#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera()
: target(0.0f, 0.0f, 0.0f), distance(6.0f), yaw(0.0f), pitch(-20.0f)
{}

glm::mat4 Camera::getViewMatrix() const {
    glm::vec3 dir;
    dir.x = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    dir.y = sin(glm::radians(pitch));
    dir.z = cos(glm::radians(pitch)) * cos(glm::radians(yaw));

    glm::vec3 camPos = target - dir * distance;
    return glm::lookAt(camPos, target, glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 Camera::getProjectionMatrix(float aspect) const {
    return glm::perspective(glm::radians(60.0f), aspect, 0.1f, 1000.0f);
}

void Camera::setTarget(const glm::vec3 &t) {
    target = t;
}

void Camera::update(float /*dt*/) {
    // simple damping or smoothing could go here later
}

void Camera::setYawPitch(float y, float p) { yaw = y; pitch = p; }
void Camera::addYawPitch(float dyaw, float dpitch) { yaw += dyaw; pitch += dpitch; if (pitch > 89.0f) pitch = 89.0f; if (pitch < -89.0f) pitch = -89.0f; }
