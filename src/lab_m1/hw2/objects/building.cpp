#include "building.h"

#include <random>

#include "../utils/render.h"

using namespace std;

Building::Building(Mesh *mesh, custom::Material *material, glm::vec3 position, glm::vec3 scale) : CollidableObject(position, scale.x / 2) {
    visuals = { mesh, material };

    SetScale(scale);
}

bool Building::CollidesWith(CollidableObject *other) {
    return other->GetCenter().x - other->GetRadius() < GetCenter().x + GetScale().x / 2 &&
           other->GetCenter().x + other->GetRadius() > GetCenter().x - GetScale().x / 2 &&
           other->GetCenter().z - other->GetRadius() < GetCenter().z + GetScale().z / 2 &&
           other->GetCenter().z + other->GetRadius() > GetCenter().z - GetScale().z / 2;
}

Building *Building::GenerateNewBuilding(
    Mesh *mesh, custom::Material *material, glm::vec3 groundLevel,
    CollidableObject *refObject
    ) {
    random_device rd;
    mt19937 gen(rd());

    uniform_int_distribution<> disBuildingPos(-50, 50);
    uniform_int_distribution<> disBuildingScale(10, 15);

    float x = disBuildingPos(gen);
    float z = disBuildingPos(gen);
    glm::vec3 scale = glm::vec3(
                                static_cast<float>(disBuildingScale(gen)), static_cast<float>(disBuildingScale(gen)),
                                static_cast<float>(disBuildingScale(gen))
                               );

    Building *building = new Building(mesh, material, glm::vec3(x, 0, z) + groundLevel, scale / 2.0f);

    while (building->CollidesWith(refObject)) {
        x = disBuildingPos(gen);
        z = disBuildingPos(gen);
        scale = glm::vec3(
                          static_cast<float>(disBuildingScale(gen)), static_cast<float>(disBuildingScale(gen)),
                          static_cast<float>(disBuildingScale(gen))
                         );

        building->SetCenter(glm::vec3(x, 0, z) + groundLevel);
        building->SetScale(scale);
        building->SetRadius(scale.x / 2);
    }

    return building;
}

void Building::Draw(custom::Camera *camera, Shader *shader, glm::vec3 lightPosition) {
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, GetCenter());
    modelMatrix = glm::scale(modelMatrix, GetScale());

    custom::RenderMesh(visuals.first, visuals.second, camera, shader, modelMatrix, lightPosition, INVALID_VALUE);
}
