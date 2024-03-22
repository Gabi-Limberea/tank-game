#pragma once

#include "core/gpu/mesh.h"
#include "core/gpu/shader.h"
#include "lab_m1/hw2/visuals/camera.h"
#include "lab_m1/hw2/visuals/material.h"
#include "primitives/object.h"

class Building : public CollidableObject {
    std::pair<Mesh *, custom::Material *> visuals;

public:
    explicit Building(Mesh *mesh, custom::Material *material, glm::vec3 position, glm::vec3 scale);

    bool CollidesWith(CollidableObject *other);
    void Draw(custom::Camera *camera, Shader *shader, glm::vec3 lightPosition);

    static Building *GenerateNewBuilding(
        Mesh *mesh, custom::Material *material, glm::vec3 groundLevel,
        CollidableObject *player
        );
};
