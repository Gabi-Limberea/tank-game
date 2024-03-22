#include "player.h"


using namespace std;

Player::Player(
    const glm::vec3 primaryColor, const glm::vec3 secondaryColor, const glm::vec3 position,
    const unordered_map<string, Mesh *> &meshes, const TankType type
    ) {
    projectileSpeed = 2.5f;
    health = MAX_HEALTH;

    tank = new Tank(position, 2.0f, MAX_HEALTH, type);

    switch (type) {
        case TankType::BASIC:
            tank->UseBasicTank(primaryColor, secondaryColor, meshes);
            break;
    }
}

void Player::Shoot() {
    auto *temp = new Projectile(
                                glm::vec3(tank->GetCannonModelMatrix() * glm::vec4(BASIC_CANNON_MOUTH_OFFSET, 1.0f)),
                                this->GetTank()->GetShootingTrajectory() * 7.5f, 0.35f
                               );
    temp->SetScale(tank->GetScale());

    projectiles.push_back(temp);
}

void Player::Draw(custom::Camera *camera, Shader *shader, const glm::vec3 lightPosition) const {
    tank->Draw(camera, shader, lightPosition);

    for (auto &projectile: projectiles) { projectile->Draw(camera, shader, lightPosition); }
}

void Player::TakeDamage() {
    health -= 10;
    tank->SetHealth(health);
}

bool Player::IsDead() const { return health <= 0; }
