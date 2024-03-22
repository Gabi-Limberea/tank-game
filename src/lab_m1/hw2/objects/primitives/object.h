#pragma once

#include "utils/glm_utils.h"

#define INVALID_VALUE (-1.0f)

class Object {
protected:
    glm::vec3 center;
    glm::vec3 scale;

    glm::vec3 up, forward, right;
    float angleOX, angleOY, angleOZ;

public:
    Object() : center(glm::vec3(0.0f)), scale(glm::vec3(1.0f)), up(glm::vec3(0.0f)), forward(glm::vec3(0.0f)),
               right(glm::vec3(0.0f)), angleOX(0.0f), angleOY(0.0f), angleOZ(0.0f) {}

    Object(glm::vec3 center) : center(center), scale(glm::vec3(1.0f)), up(glm::vec3(0, 1, 0)), forward(glm::vec3(1, 0, 0)),
                               right(glm::vec3(0, 0, 1)), angleOX(0.0f), angleOY(0.0f), angleOZ(0.0f) {}

    ~Object() {}

    glm::vec3 GetCenter() { return center; }
    glm::vec3 GetScale() { return scale; }
    glm::vec3 GetUp() { return up; }
    glm::vec3 GetForward() { return forward; }
    glm::vec3 GetRight() { return right; }
    float GetAngleOX() { return angleOX; }
    float GetAngleOY() { return angleOY; }
    float GetAngleOZ() { return angleOZ; }

    virtual void SetCenter(glm::vec3 center) { this->center = center; }
    void SetScale(glm::vec3 scale) { this->scale = scale; }
    void SetAngleOX(float angleOX) { this->angleOX = angleOX; }
    void SetAngleOY(float angleOY) { this->angleOY = angleOY; }
    void SetAngleOZ(float angleOZ) { this->angleOZ = angleOZ; }
    void SetForward(glm::vec3 forward) { this->forward = forward; }
    void SetUp(glm::vec3 up) { this->up = up; }
    void SetRight(glm::vec3 right) { this->right = right; }

    void MoveForward(float distance);
    void TranslateForward(float distance);
    void TranslateUpward(float distance);
    void TranslateRight(float distance);

    void RotateOX(float angle);
    void RotateOY(float angle);
    void RotateOZ(float angle);
};

class CollidableObject : public Object {
protected:
    float radius;

public:
    CollidableObject() : radius(0.0f) {}
    CollidableObject(glm::vec3 center, float radius) : Object(center), radius(radius) {}

    ~CollidableObject() {}

    float GetRadius() { return radius; }

    void SetRadius(float radius) { this->radius = radius; }

    virtual bool CollidesWith(const CollidableObject *other);
};

class Removable {
    bool needsToBeRemoved;

public:
    Removable() : needsToBeRemoved(false) {}
    ~Removable() {}

    bool NeedsToBeRemoved() { return needsToBeRemoved; }
    void SetRemoveState(bool state) { needsToBeRemoved = state; }

    virtual void TryRemove(bool otherCondition) = 0;
};
