#include "tank.h"

#include "lab_m1/hw2/visuals/colors.h"
#include "../utils/render.h"

using namespace std;

Tank::Tank(glm::vec3 center, float speed, float health, TankType type) : CollidableObject(center, 0.0f), speed(speed), health(health) {
    switch (type) {
        case TankType::BASIC:
            bigWheel = new Object(center + BASIC_BIG_WHEEL_OFFSET);
            smallWheel = new Object(center + BASIC_SMALL_WHEEL_OFFSET);
            cannon = new Object(center + BASIC_CANNON_OFFSET);
            turret = new Object(center);

            break;
    }
}

void Tank::LoadMeshes(
    vector<std::pair<Mesh *, custom::Material *>> bodyVisuals,
    vector<std::pair<Mesh *, custom::Material *>> turretVisuals,
    vector<std::pair<Mesh *, custom::Material *>> cannonVisuals,
    vector<std::pair<Mesh *, custom::Material *>> tracksVisuals,
    pair<Mesh *, custom::Material *> bigWheelVisuals,
    pair<Mesh *, custom::Material *> smallWheelVisuals
    ) {
    this->bodyVisuals = bodyVisuals;
    this->turretVisuals = turretVisuals;
    this->cannonVisuals = cannonVisuals;
    this->tracksVisuals = tracksVisuals;
    this->bigWheelVisuals = bigWheelVisuals;
    this->smallWheelVisuals = smallWheelVisuals;
}

void Tank::DrawBody(custom::Camera *camera, Shader *shader, glm::vec3 lightPosition) {
    for (auto &piece: bodyVisuals) {
        glm::mat4 modelMatrix = glm::mat4(1.0f);

        modelMatrix = glm::translate(modelMatrix, center);
        modelMatrix = glm::scale(modelMatrix, scale);
        modelMatrix = glm::rotate(modelMatrix, angleOY, glm::vec3(0, 1, 0));

        custom::RenderMesh(piece.first, piece.second, camera, shader, modelMatrix, lightPosition, health);
    }
}

void Tank::DrawTurret(custom::Camera *camera, Shader *shader, glm::vec3 lightPosition) {
    for (auto &piece: turretVisuals) {
        glm::mat4 modelMatrix = glm::mat4(1.0f);

        modelMatrix = glm::translate(modelMatrix, center);
        modelMatrix = glm::scale(modelMatrix, scale);
        modelMatrix = glm::rotate(modelMatrix, angleOY, glm::vec3(0, 1, 0));

        modelMatrix = glm::translate(modelMatrix, turret->GetCenter() - center);
        modelMatrix = glm::rotate(modelMatrix, turret->GetAngleOY(), glm::vec3(0, 1, 0));
        modelMatrix = glm::translate(modelMatrix, -(turret->GetCenter() - center));

        custom::RenderMesh(piece.first, piece.second, camera, shader, modelMatrix, lightPosition, health);
    }
}

void Tank::DrawCannon(custom::Camera *camera, Shader *shader, glm::vec3 lightPosition) {
    for (auto &piece: cannonVisuals) {
        glm::mat4 modelMatrix = glm::mat4(1.0f);

        modelMatrix = glm::translate(modelMatrix, center);
        modelMatrix = glm::scale(modelMatrix, scale);
        modelMatrix = glm::rotate(modelMatrix, angleOY, glm::vec3(0, 1, 0));

        modelMatrix = glm::translate(modelMatrix, turret->GetCenter() - center);
        modelMatrix = glm::rotate(modelMatrix, turret->GetAngleOY(), glm::vec3(0, 1, 0));
        modelMatrix = glm::translate(modelMatrix, -(turret->GetCenter() - center));

        modelMatrix = glm::translate(modelMatrix, cannon->GetCenter() - center);
        modelMatrix = glm::rotate(modelMatrix, cannon->GetAngleOX(), glm::vec3(0, 0, 1));
        modelMatrix = glm::translate(modelMatrix, -(cannon->GetCenter() - center));

        custom::RenderMesh(piece.first, piece.second, camera, shader, modelMatrix, lightPosition, health);
    }
}

void Tank::DrawTracks(custom::Camera *camera, Shader *shader, glm::vec3 lightPosition) {
    for (auto &piece: tracksVisuals) {
        glm::mat4 modelMatrix = glm::mat4(1.0f);

        modelMatrix = glm::translate(modelMatrix, center);
        modelMatrix = glm::scale(modelMatrix, scale);
        modelMatrix = glm::rotate(modelMatrix, angleOY, up);

        custom::RenderMesh(piece.first, piece.second, camera, shader, modelMatrix, lightPosition, health);
    } {
        glm::mat4 modelMatrix = glm::mat4(1.0f);

        modelMatrix = glm::translate(modelMatrix, center);
        modelMatrix = glm::scale(modelMatrix, scale);
        modelMatrix = glm::rotate(modelMatrix, angleOY, up);

        modelMatrix = glm::translate(modelMatrix, bigWheel->GetCenter() - center);
        modelMatrix = glm::rotate(modelMatrix, bigWheel->GetAngleOX(), bigWheel->GetRight());
        modelMatrix = glm::translate(modelMatrix, -(bigWheel->GetCenter() - center));

        custom::RenderMesh(bigWheelVisuals.first, bigWheelVisuals.second, camera, shader, modelMatrix, lightPosition, health);
    } {
        glm::mat4 modelMatrix = glm::mat4(1.0f);

        modelMatrix = glm::translate(modelMatrix, center);
        modelMatrix = glm::scale(modelMatrix, scale);
        modelMatrix = glm::rotate(modelMatrix, angleOY, up);

        modelMatrix = glm::translate(modelMatrix, smallWheel->GetCenter() - center);
        modelMatrix = glm::rotate(modelMatrix, smallWheel->GetAngleOX(), smallWheel->GetRight());
        modelMatrix = glm::translate(modelMatrix, -(smallWheel->GetCenter() - center));

        custom::RenderMesh(smallWheelVisuals.first, smallWheelVisuals.second, camera, shader, modelMatrix, lightPosition, health);
    }
}

void Tank::Draw(custom::Camera *camera, Shader *shader, glm::vec3 lightPosition) {
    DrawBody(camera, shader, lightPosition);
    DrawTurret(camera, shader, lightPosition);
    DrawCannon(camera, shader, lightPosition);
    DrawTracks(camera, shader, lightPosition);
}

void Tank::Move(float deltaTimeSeconds) {
    float distance = deltaTimeSeconds * speed;
    float wheelAngle = deltaTimeSeconds * RADIANS(60);
    glm::vec3 dir = glm::normalize(glm::vec3(forward.x, 0, forward.z));

    MoveForward(distance);

    bigWheel->SetCenter(bigWheel->GetCenter() + dir * distance);
    bigWheel->SetAngleOX(bigWheel->GetAngleOX() - wheelAngle * speed);

    smallWheel->SetCenter(smallWheel->GetCenter() + dir * distance);
    smallWheel->SetAngleOX(smallWheel->GetAngleOX() + wheelAngle * speed);

    cannon->SetCenter(cannon->GetCenter() + dir * distance);
    turret->SetCenter(turret->GetCenter() + dir * distance);
}

void Tank::Rotate(float angle) {
    RotateOY(angle);
    SetAngleOY(GetAngleOY() + angle);

    turret->RotateOY(angle);
    cannon->RotateOY(angle);
}

void Tank::RotateTurret(float angle) {
    if (turret->GetAngleOY() + angle <= RADIANS(60) && turret->GetAngleOY() + angle >= RADIANS(-60)) {
        turret->RotateOY(angle);
        turret->SetAngleOY(turret->GetAngleOY() + angle);

        cannon->RotateOY(angle);
    } else if (turret->GetAngleOY() + angle > RADIANS(60)) {
        if (turret->GetAngleOY() < RADIANS(60)) {
            turret->RotateOY(RADIANS(60) - turret->GetAngleOY());
            cannon->RotateOY(RADIANS(60) - turret->GetAngleOY());
        }

        turret->SetAngleOY(RADIANS(60));
    } else if (turret->GetAngleOY() + angle < RADIANS(-60)) {
        if (turret->GetAngleOY() > RADIANS(-60)) {
            turret->RotateOY(RADIANS(-60) - turret->GetAngleOY());
            cannon->RotateOY(RADIANS(-60) - turret->GetAngleOY());
        }

        turret->SetAngleOY(RADIANS(-60));
    }
}

void Tank::RotateCannon(float angle) {
    if (cannon->GetAngleOX() + angle <= RADIANS(45) && cannon->GetAngleOX() + angle >= RADIANS(-15)) {
        cannon->RotateOX(angle);
        cannon->SetAngleOX(cannon->GetAngleOX() + angle);
    } else if (cannon->GetAngleOX() + angle > RADIANS(45)) {
        if (cannon->GetAngleOX() < RADIANS(45)) { cannon->RotateOX(RADIANS(45) - cannon->GetAngleOX()); }

        cannon->SetAngleOX(RADIANS(45));
    } else if (cannon->GetAngleOX() + angle < RADIANS(-15)) {
        if (cannon->GetAngleOX() > RADIANS(-15)) { cannon->RotateOX(RADIANS(-15) - cannon->GetAngleOX()); }

        cannon->SetAngleOX(RADIANS(-15));
    }
}

void Tank::UseBasicTank(glm::vec3 primaryColor, glm::vec3 secondaryColor, unordered_map<string, Mesh *> meshes) {
    this->SetScale(glm::vec3(0.1f));
    this->SetRadius(BASIC_TANK_RADIUS * 0.1f);

    custom::Material *primaryColorMaterial = new custom::Material(0.05f, 0.5f, 0.75f, 0.0f, 0.55f, primaryColor);
    custom::Material *secondaryColorMaterial = new custom::Material(0.05f, 0.5f, 0.75f, 0.0f, 0.55f, secondaryColor);
    custom::Material *yellowMaterial = new custom::Material(0.05f, 0.5f, 0.75f, 0.2f, 0.55f, BRASS);
    custom::Material *lightGrayMaterial = new custom::Material(0.05f, 0.5f, 0.75f, 0.0f, 0.55f, LIGHT_GRAY);
    custom::Material *darkGrayMaterial = new custom::Material(0.05f, 0.5f, 0.75f, 0.0f, 0.55f, DARK_GRAY);

    this->LoadMeshes(
                     {
                         { meshes["tank_body"], primaryColorMaterial },
                         { meshes["tank_body_details_1"], lightGrayMaterial },
                         { meshes["tank_body_details_2"], darkGrayMaterial },
                         { meshes["tank_headlights"], yellowMaterial }
                     },
                     {
                         { meshes["tank_turret"], secondaryColorMaterial },
                         { meshes["tank_turret_details_1"], darkGrayMaterial }
                     },
                     {
                         { meshes["tank_cannon"], darkGrayMaterial },
                         { meshes["tank_cannon_details_1"], secondaryColorMaterial }
                     },
                     {
                         { meshes["tank_tracks"], darkGrayMaterial },
                         { meshes["tank_tracks_connectors"], lightGrayMaterial },
                     },
                     { meshes["tank_big_wheel"], darkGrayMaterial },
                     { meshes["tank_small_wheel"], darkGrayMaterial }
                    );
}

glm::mat4 Tank::GetCannonModelMatrix() {
    glm::mat4 modelMatrix = glm::mat4(1.0f);

    modelMatrix = glm::translate(modelMatrix, center);
    modelMatrix = glm::scale(modelMatrix, scale);
    modelMatrix = glm::rotate(modelMatrix, angleOY, glm::vec3(0, 1, 0));

    modelMatrix = glm::translate(modelMatrix, turret->GetCenter() - center);
    modelMatrix = glm::rotate(modelMatrix, turret->GetAngleOY(), glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, -(turret->GetCenter() - center));

    modelMatrix = glm::translate(modelMatrix, cannon->GetCenter() - center);
    modelMatrix = glm::rotate(modelMatrix, cannon->GetAngleOX(), glm::vec3(0, 0, 1));
    modelMatrix = glm::translate(modelMatrix, -(cannon->GetCenter() - center));

    return modelMatrix;
}

bool Tank::TurretCanStillMove(float targetAngle) {
    return turret->GetAngleOY() + targetAngle > RADIANS(-60) && turret->GetAngleOY() + targetAngle < RADIANS(60);
}
