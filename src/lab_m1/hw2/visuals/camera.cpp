#include "camera.h"

using namespace custom;

void Camera::MoveForward(float distance) {
    // Translates the camera using the `dir` vector computed from
    // `forward`. Movement will always keep the camera at the same
    // height. For example, if you rotate your head up/down, and then
    // walk forward, then you will still keep the same relative
    // distance (height) to the ground!
    glm::vec3 dir = glm::normalize(glm::vec3(forward.x, 0, forward.z));
    position += dir * distance;
}

void Camera::TranslateForward(float distance) { position += forward * distance; }

void Camera::TranslateUpward(float distance) { position += up * distance; }

void Camera::TranslateRight(float distance) {
    // You need to translate the camera using the `right` vector.
    // Usually, however, translation using camera's `right` vector
    // is not very useful, because if the camera is rotated around the
    // `forward` vector, then the translation on the `right` direction
    // will have an undesired effect, more precisely, the camera will
    // get closer or farther from the ground. The solution is to
    // actually use the projected `right` vector (projected onto the
    // ground plane), which makes more sense because we will keep the
    // same distance from the ground plane.
    position += glm::normalize(glm::vec3(right.x, 0, right.z)) * distance;
}

void Camera::RotateFirstPerson_OX(float angle) {
    forward = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), angle, right) * glm::vec4(forward, 1)));
    up = glm::cross(right, forward);
}

void Camera::RotateFirstPerson_OY(float angle) {
    forward = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), angle, glm::vec3(0, 1, 0)) * glm::vec4(forward, 1)));
    right = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), angle, glm::vec3(0, 1, 0)) * glm::vec4(right, 1)));
    up = glm::cross(right, forward);
}

void Camera::RotateFirstPerson_OZ(float angle) {
    right = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), angle, forward) * glm::vec4(right, 1)));
    up = glm::cross(right, forward);
}

void Camera::RotateThirdPerson_OX(float angle) {
    TranslateForward(distanceToTarget);
    RotateFirstPerson_OX(angle);
    TranslateForward(-distanceToTarget);
}

void Camera::RotateThirdPerson_OY(float angle) {
    TranslateForward(distanceToTarget);
    RotateFirstPerson_OY(angle);
    TranslateForward(-distanceToTarget);
}

void Camera::RotateThirdPerson_OZ(float angle) {
    TranslateForward(distanceToTarget);
    RotateFirstPerson_OY(angle);
    TranslateForward(-distanceToTarget);
}

glm::mat4 Camera::GetViewMatrix() { return glm::lookAt(position, position + forward, up); }

glm::vec3 Camera::GetTargetPosition() { return position + forward * distanceToTarget; }

glm::mat4 Camera::GetProjectionMatrix() { return glm::perspective(fov, aspectRatio, zNear, zFar); }
