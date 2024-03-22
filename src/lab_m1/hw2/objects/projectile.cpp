#include "projectile.h"

#include "../utils/render.h"

using namespace std;

pair<Mesh *, custom::Material *> Projectile::visuals = { nullptr, nullptr };

void Projectile::LoadVisuals(Mesh *mesh, custom::Material *material) { visuals = { mesh, material }; }

void Projectile::Move(float deltaTime) {
    velocity += GRAVITY * deltaTime;
    center += velocity * deltaTime;
}

void Projectile::Draw(custom::Camera *camera, Shader *shader, glm::vec3 lightPosition) {
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, center);
    modelMatrix = glm::scale(modelMatrix, scale);

    custom::RenderMesh(visuals.first, visuals.second, camera, shader, modelMatrix, lightPosition, INVALID_VALUE);
}

void Projectile::TryRemove(bool otherCondition) { if (otherCondition || center.y < 0.0f) { SetRemoveState(true); } }
