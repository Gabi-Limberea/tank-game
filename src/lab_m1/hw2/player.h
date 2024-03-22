#pragma once

#include <unordered_map>

#include "lab_m1/hw2/objects/projectile.h"
#include "lab_m1/hw2/objects/tank.h"
#include "utils/glm_utils.h"

#define MAX_HEALTH 100.0f

class Player {
    float health;
    Tank *tank;
    float projectileSpeed;

public:
    std::vector<Projectile *> projectiles;

    explicit Player(
        glm::vec3 primaryColor, glm::vec3 secondaryColor, glm::vec3 position,
        const std::unordered_map<std::string, Mesh *> &meshes, TankType type
        );

    void SetTank(Tank *tank) { this->tank = tank; }
    void SetHealth(const float health) { this->health = health; }

    float GetHealth() const { return health; }
    Tank *GetTank() { return tank; }

    void TakeDamage();
    void Shoot();
    void Draw(custom::Camera *camera, Shader *shader, glm::vec3 lightPosition) const;
    bool IsDead() const;
};
