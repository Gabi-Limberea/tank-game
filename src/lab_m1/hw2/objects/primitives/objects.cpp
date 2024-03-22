#include "object.h"

void Object::MoveForward(float distance) {
    glm::vec3 dir = glm::normalize(glm::vec3(forward.x, 0, forward.z));
    center += dir * distance;
}

void Object::TranslateForward(float distance) { center += forward * distance; }

void Object::TranslateUpward(float distance) { center += up * distance; }

void Object::TranslateRight(float distance) { center += glm::normalize(glm::vec3(right.x, 0, right.z)) * distance; }

void Object::RotateOX(float angle) {
    forward = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), angle, right) * glm::vec4(forward, 1)));
    up = glm::cross(right, forward);
}

void Object::RotateOY(float angle) {
    forward = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), angle, glm::vec3(0, 1, 0)) * glm::vec4(forward, 1)));
    right = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), angle, glm::vec3(0, 1, 0)) * glm::vec4(right, 1)));
    up = glm::cross(right, forward);
}

void Object::RotateOZ(float angle) {
    right = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), angle, forward) * glm::vec4(right, 1)));
    up = glm::cross(right, forward);
}

bool CollidableObject::CollidesWith(const CollidableObject *other) {
    return glm::distance(this->center, other->center) < this->radius + other->radius;
}
