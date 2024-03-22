#pragma once

#include "player.h"
#include "lab_m1/hw2/objects/building.h"
#include "lab_m1/hw2/utils/timer.h"

typedef enum State {
    NO_MOVE,
    MOVE_FORWARD,
    ROTATE_LEFT,
    ROTATE_RIGHT,
    MOVE_BACKWARD,
} State;

class Enemy : public Player {
    State state;
    Timer *stateTimer;
    static glm::vec3 primaryColor, secondaryColor;

    Enemy(
        const glm::vec3 position, const std::unordered_map<std::string, Mesh *> &meshes, const TankType type
        ) : Player(primaryColor, secondaryColor, position, meshes, type), state(State::NO_MOVE), stateTimer(new Timer(0.001f)),
            shootTimer(new Timer(5)) {}

public:
    Timer *shootTimer;

    State GetState() const { return state; }

    void AutoMove(float deltaTime);
    void AutoShoot();
    void Stall(float deltaTime);

    static void LoadVisuals(glm::vec3 primaryColor, glm::vec3 secondaryColor);
    static Enemy *GenerateEnemy(
        const std::vector<Building *> &buildings,
        const CollidableObject *refObject, const std::unordered_map<std::string, Mesh *> &meshes, const glm::vec3 &groundLevel
        );
};
