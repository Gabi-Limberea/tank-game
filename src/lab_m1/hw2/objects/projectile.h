#pragma once

#include "core/gpu/mesh.h"
#include "core/gpu/shader.h"
#include "lab_m1/hw2/visuals/camera.h"
#include "lab_m1/hw2/visuals/material.h"
#include "primitives/object.h"

#define GRAVITY glm::vec3(0.0f, -9.81f, 0.0f)

class Projectile : public CollidableObject, public Removable {
    static std::pair<Mesh *, custom::Material *> visuals;
    glm::vec3 velocity;

public:
    Projectile(glm::vec3 position, glm::vec3 velocity, float radius) : CollidableObject(position, radius), velocity(velocity) {};

    glm::vec3 GetVelocity() const { return velocity; }

    void SetVelocity(glm::vec3 velocity) { this->velocity = velocity; }

    void Move(float deltaTime);
    void Draw(custom::Camera *camera, Shader *shader, glm::vec3 lightPosition);

    void TryRemove(bool otherCondition) override;

    static void LoadVisuals(Mesh *mesh, custom::Material *material);
};
