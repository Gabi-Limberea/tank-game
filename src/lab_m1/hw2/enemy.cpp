#include "enemy.h"

#include <random>

using namespace std;

glm::vec3 Enemy::primaryColor;
glm::vec3 Enemy::secondaryColor;

void Enemy::LoadVisuals(const glm::vec3 primaryColor, const glm::vec3 secondaryColor) {
    Enemy::primaryColor = primaryColor;
    Enemy::secondaryColor = secondaryColor;
}

void Enemy::AutoMove(const float deltaTime) {
    if (stateTimer->IsDone()) {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> disState(NO_MOVE, MOVE_BACKWARD);

        state = static_cast<State>(disState(gen));
        stateTimer->Reset();
    }

    switch (state) {
        case NO_MOVE:
            break;
        case MOVE_FORWARD:
            GetTank()->Move(deltaTime);
            break;
        case ROTATE_LEFT:
            GetTank()->Rotate(-deltaTime * RADIANS(15));
            break;
        case ROTATE_RIGHT:
            GetTank()->Rotate(deltaTime * RADIANS(15));
            break;
        case MOVE_BACKWARD:
            GetTank()->Move(-deltaTime);
            break;
        default:
            break;
    }

    stateTimer->Update(deltaTime);
}

void Enemy::AutoShoot() {
    if (shootTimer->IsDone()) {
        Shoot();
        shootTimer->HardReset();
    }
}

void Enemy::Stall(float deltaTime) {
    if (shootTimer->IsDone()) {
        return;
    }

    shootTimer->Update(deltaTime);
}

Enemy *Enemy::GenerateEnemy(
    const std::vector<Building *> &buildings,
    const CollidableObject *refObject, const std::unordered_map<std::string, Mesh *> &meshes, const glm::vec3 &groundLevel
    ) {
    random_device rd;
    mt19937 gen(rd());

    uniform_int_distribution<> disEnemyPos(-30, 30);
    uniform_real_distribution<> disEnemyRotation(-180, 180);
    uniform_int_distribution<> disEnemyStateTimer(2, 5);
    uniform_int_distribution<> disEnemyState(NO_MOVE, MOVE_BACKWARD);

    float x = disEnemyPos(gen);
    float z = disEnemyPos(gen);
    float radians = RADIANS(disEnemyRotation(gen));

    auto enemy = new Enemy(glm::vec3(x, 0, z) + groundLevel, meshes, TankType::BASIC);
    bool collides;

    do {
        collides = false;

        if (enemy->GetTank()->CollidesWith(refObject)) {
            collides = true;
            break;
        }

        for (auto &building: buildings) {
            if (building->CollidesWith(enemy->GetTank())) {
                collides = true;

                break;
            }
        }

        if (collides) {
            x = disEnemyPos(gen);
            z = disEnemyPos(gen);

            enemy->GetTank()->SetCenter(glm::vec3(x, 0, z) + groundLevel);
        }
    } while (collides);

    enemy->GetTank()->Rotate(radians);
    enemy->state = static_cast<State>(disEnemyState(gen));
    enemy->stateTimer = new Timer(disEnemyStateTimer(gen));

    return enemy;
}
