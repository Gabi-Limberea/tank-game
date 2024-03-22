#pragma once

#include <unordered_map>

#include "core/gpu/mesh.h"
#include "core/gpu/shader.h"
#include "lab_m1/hw2/objects/primitives/object.h"
#include "lab_m1/hw2/visuals/camera.h"
#include "lab_m1/hw2/visuals/material.h"

#define MAX_HEALTH 100.0f

typedef enum TankType {
    BASIC,
} TankType;

#define BASIC_BIG_WHEEL_OFFSET  glm::vec3(1.61945f, -2.25596f, -0.000003f)
#define BASIC_SMALL_WHEEL_OFFSET glm::vec3(-2.1131f, -2.6135f, 0.0f)
#define BASIC_CANNON_OFFSET	 glm::vec3(1.5f, 5.0f, 0.0f)
#define BASIC_TANK_RADIUS 6.5f
#define BASIC_CANNON_MOUTH_OFFSET glm::vec3(8.75728f, 4.5f, 0.0f)

class Tank : public CollidableObject {
    Object *bigWheel, *smallWheel, *cannon, *turret; // objects that also move independently
    std::vector<std::pair<Mesh *, custom::Material *>> bodyVisuals;
    std::vector<std::pair<Mesh *, custom::Material *>> turretVisuals;
    std::vector<std::pair<Mesh *, custom::Material *>> cannonVisuals;
    std::vector<std::pair<Mesh *, custom::Material *>> tracksVisuals;
    std::pair<Mesh *, custom::Material *> bigWheelVisuals;
    std::pair<Mesh *, custom::Material *> smallWheelVisuals;

    float speed = 2.0f;
    float health = 100.0f;

    void DrawBody(custom::Camera *camera, Shader *shader, glm::vec3 lightPosition);
    void DrawTurret(custom::Camera *camera, Shader *shader, glm::vec3 lightPosition);
    void DrawCannon(custom::Camera *camera, Shader *shader, glm::vec3 lightPosition);
    void DrawTracks(custom::Camera *camera, Shader *shader, glm::vec3 lightPosition);

public:
    explicit Tank(glm::vec3 center, float speed, float health, TankType type);

    void LoadMeshes(
        std::vector<std::pair<Mesh *, custom::Material *>> bodyVisuals,
        std::vector<std::pair<Mesh *, custom::Material *>> turretVisuals,
        std::vector<std::pair<Mesh *, custom::Material *>> cannonVisuals,
        std::vector<std::pair<Mesh *, custom::Material *>> tracksVisuals,
        std::pair<Mesh *, custom::Material *> bigWheelVisuals,
        std::pair<Mesh *, custom::Material *> smallWheelVisuals
        );

    void Draw(custom::Camera *camera, Shader *shader, glm::vec3 lightPosition);
    void Move(float deltaTimeSeconds);
    void Rotate(float angle);
    void RotateTurret(float angle);
    void RotateCannon(float angle);

    float GetHealth() { return health; }
    float GetSpeed() { return speed; }
    glm::vec3 GetShootingTrajectory() { return cannon->GetForward(); }
    glm::mat4 GetCannonModelMatrix();

    void SetHealth(float health) { this->health = health; }
    void SetSpeed(float speed) { this->speed = speed; }
    void SetCenter(glm::vec3 center) override {
        Object::SetCenter(center);
        bigWheel->SetCenter(center + BASIC_BIG_WHEEL_OFFSET);
        smallWheel->SetCenter(center + BASIC_SMALL_WHEEL_OFFSET);
        turret->SetCenter(center);
        cannon->SetCenter(center + BASIC_CANNON_OFFSET);
    }

    bool TurretCanStillMove(float targetAngle);

    // funcs like these should be added if we want to use other tank meshes
    void UseBasicTank(glm::vec3 primaryColor, glm::vec3 secondaryColor, std::unordered_map<std::string, Mesh *> meshes);
};
