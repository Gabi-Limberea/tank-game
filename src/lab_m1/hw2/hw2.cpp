#include "lab_m1/hw2/hw2.h"

#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "lab_m1/hw2/visuals/colors.h"
#include "utils/render.h"
#include "glm/gtx/vector_angle.hpp"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Hw2::Hw2() {}


Hw2::~Hw2() = default;


void Hw2::Init() {
    timer = 0.0f;
    deadEnemies = 0;

    camera = new custom::Camera(
                                glm::vec3(-2.75f, 1.5f, 0), glm::vec3(0, 0.4825f, 0), glm::vec3(0, 1, 0), 3, RADIANS(60),
                                window->props.aspectRatio, 0.01f, 200.0f
                               );
    groundLevel = glm::vec3(0.0f, camera->GetTargetPosition().y, 0.0f);

    textRenderer = new gfxc::TextRenderer(window->props.selfDir, window->GetResolution().x, window->GetResolution().y);
    textRenderer->Load(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::FONTS, "Hack-Bold.ttf"), 24);

    AddTankMeshes();

    auto mesh = new Mesh("plane");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
    meshes[mesh->GetMeshID()] = mesh;
    dirt = new custom::Material(0.3f, 0.1f, 0.1f, 0.0f, 0.55f, BROWN);

    mesh = new Mesh("cube");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
    meshes[mesh->GetMeshID()] = mesh;

    mesh = new Mesh("projectile");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tank components"), "projectile.obj");
    meshes[mesh->GetMeshID()] = mesh;
    auto *temp = new custom::Material(0.2f, 0.2f, 0.2f, 0.0f, 0.55f, DARK_GRAY);

    Projectile::LoadVisuals(meshes["projectile"], temp);

    lightPosition = glm::vec3(0, 5, 0) + camera->GetTargetPosition();

    player = new Player(DARK_GREEN, GREEN, camera->GetTargetPosition(), meshes, TankType::BASIC);

    Enemy::LoadVisuals(PINK, PURPLE);

    GenerateBuildings();
    GenerateEnemies();
    InitShaders();
}


void Hw2::FrameStart() {
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.71, 0.824, 0.922, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Hw2::Update(float deltaTimeSeconds) {
    timer += deltaTimeSeconds;

    if (!player->IsDead() && deadEnemies != enemies.size() && timer < 300.0f) {
        HandlePlayerProjectiles(deltaTimeSeconds);
        HandleEnemies(deltaTimeSeconds);
    }

    RenderBuildings();
    RenderEnemies();
    custom::RenderMesh(
                       meshes["plane"], dirt, camera, shaders["BasicShader"], glm::scale(glm::mat4(1), glm::vec3(2)), lightPosition,
                       INVALID_VALUE
                      );
    player->Draw(camera, shaders["BasicShader"], lightPosition);
}


void Hw2::FrameEnd() {
    textRenderer->RenderText("Health: " + to_string(static_cast<int>(player->GetHealth())), 10, 10, 1, glm::vec3(0, 0, 0));
    textRenderer->RenderText("Enemies alive:" + to_string(enemies.size() - deadEnemies), 10, 50, 1, glm::vec3(0, 0, 0));
    textRenderer->RenderText("Score:" + to_string(deadEnemies * 100), 10, 70, 1, glm::vec3(0, 0, 0));

    if (timer >= 300.0f) {
        textRenderer->RenderText("Time's up!", 10, 30, 1, glm::vec3(0, 0, 0));

        return;
    }

    if (deadEnemies == enemies.size()) {
        textRenderer->RenderText("YOU WON!", 10, 30, 1, glm::vec3(0, 0, 0));

        return;
    }

    if (player->IsDead()) {
        textRenderer->RenderText("GAME OVER!", 10, 30, 1, glm::vec3(0, 0, 0));

        return;
    }

    textRenderer->RenderText("Time: " + to_string(static_cast<int>(timer)), 10, 30, 1, glm::vec3(0, 0, 0));
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Hw2::OnInputUpdate(float deltaTime, int mods) {
    // Add key press event

    int direction = 0;

    if (timer < 300.0f && !player->IsDead() && deadEnemies != enemies.size()) {
        if (window->KeyHold(GLFW_KEY_W)) {
            player->GetTank()->Move(deltaTime);
            camera->position += player->GetTank()->GetCenter() - camera->GetTargetPosition();
            direction = -1;
        }

        if (window->KeyHold(GLFW_KEY_S)) {
            player->GetTank()->Move(-deltaTime);
            camera->position += player->GetTank()->GetCenter() - camera->GetTargetPosition();
            direction = 1;
        }

        if (direction) {
            for (auto &building: buildings) {
                if (building->CollidesWith(player->GetTank())) {
                    player->GetTank()->Move(direction * deltaTime);
                    camera->position += player->GetTank()->GetCenter() - camera->GetTargetPosition();
                    break;
                }
            }

            for (auto &enemy: enemies) {
                if (enemy->GetTank()->CollidesWith(player->GetTank())) {
                    player->GetTank()->Move(direction * deltaTime);
                    camera->position += player->GetTank()->GetCenter() - camera->GetTargetPosition();
                    break;
                }
            }
        }

        if (window->KeyHold(GLFW_KEY_A)) {
            camera->RotateThirdPerson_OZ(RADIANS(60) * deltaTime);
            player->GetTank()->Rotate(RADIANS(60) * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_D)) {
            camera->RotateThirdPerson_OZ(-RADIANS(60) * deltaTime);
            player->GetTank()->Rotate(-RADIANS(60) * deltaTime);
        }
    }
}

void Hw2::OnKeyPress(int key, int mods) {
    // Add key press event
}

void Hw2::OnKeyRelease(int key, int mods) {
    // Add key release event
}

void Hw2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {
    // Add mouse move event
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
        float sensivityOX = 0.05f;
        float sensivityOY = 0.05f;

        float angleOY = -static_cast<float>(deltaX) * sensivityOX;
        float angleOX = -static_cast<float>(deltaY) * sensivityOY;

        camera->RotateThirdPerson_OX(RADIANS(angleOX));
        camera->RotateThirdPerson_OY(RADIANS(angleOY));
    } else {
        if (timer < 300.0f && !player->IsDead() && deadEnemies != enemies.size()) {
            float sensivityOX = 0.25f;
            float sensivityOY = 0.25f;

            float angleOY = -static_cast<float>(deltaX) * sensivityOX;
            float angleOX = -static_cast<float>(deltaY) * sensivityOY;

            player->GetTank()->RotateTurret(RADIANS(angleOY));
            player->GetTank()->RotateCannon(RADIANS(angleOX));
        }
    }
}

void Hw2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {
    // Add mouse button press event
    if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT) && !window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) { player->Shoot(); }
}

void Hw2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {
    // Add mouse button release event
}

void Hw2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {}

void Hw2::OnWindowResize(int width, int height) {}


void Hw2::HandleEnemies(float deltaTimeSeconds) {
    deadEnemies = 0;
    for (auto &enemy: enemies) {
        if (enemy->IsDead()) {
            deadEnemies++;
            continue;
        }

        enemy->AutoMove(deltaTimeSeconds);

        int collides = 0;

        for (auto &building: buildings) {
            if (building->CollidesWith(enemy->GetTank())) {
                if (enemy->GetState() == State::MOVE_FORWARD) {
                    collides = -1;
                } else if (enemy->GetState() == State::MOVE_BACKWARD) {
                    collides = 1;
                }

                break;
            }
        }

        if (enemy->GetTank()->CollidesWith(player->GetTank())) {
            if (enemy->GetState() == State::MOVE_FORWARD) {
                collides = -1;
            } else if (enemy->GetState() == State::MOVE_BACKWARD) {
                collides = 1;
            }
        }

        if (collides) {
            enemy->GetTank()->Move(collides * deltaTimeSeconds);
        }

        if (glm::distance(enemy->GetTank()->GetCenter(), player->GetTank()->GetCenter()) < 5.0f) {
            float angle = acos(
                               glm::dot(
                                        glm::normalize(enemy->GetTank()->GetShootingTrajectory()),
                                        glm::normalize(
                                                       player->GetTank()->GetCenter() - enemy->GetTank()->GetCenter()
                                                      )
                                       )
                              );

            if (glm::cross(
                           glm::normalize(enemy->GetTank()->GetShootingTrajectory()),
                           glm::normalize(
                                          player->GetTank()->GetCenter() - enemy->GetTank()->GetCenter()
                                         )
                          ).y < 0) {
                angle = -angle;
            }

            if ((-5 <= DEGREES(angle) && DEGREES(angle) <= 5) && enemy->GetTank()->TurretCanStillMove(angle * deltaTimeSeconds)) {
                enemy->AutoShoot();
            } else if ((-60 <= DEGREES(angle) && DEGREES(angle) <= 60) && enemy->GetTank()->TurretCanStillMove(angle * deltaTimeSeconds)) {
                enemy->GetTank()->RotateTurret(angle * deltaTimeSeconds);
            } else {
                enemy->GetTank()->Rotate(angle * deltaTimeSeconds);
            }
        }

        enemy->Stall(deltaTimeSeconds);
        HandleEnemyProjectiles(enemy, deltaTimeSeconds);
    }
}

void Hw2::RenderBuildings() {
    for (auto &building: buildings) { building->Draw(camera, shaders["BasicShader"], lightPosition); }
}

void Hw2::RenderEnemies() {
    for (auto &enemy: enemies) { enemy->Draw(camera, shaders["BasicShader"], lightPosition); }
}

void Hw2::HandlePlayerProjectiles(float deltaTimeSeconds) {
    for (auto &projectile: player->projectiles) {
        projectile->TryRemove(
                              find_if(
                                      buildings.begin(), buildings.end(),
                                      [&projectile](Building *building) -> bool {
                                          return building->CollidesWith(projectile);
                                      }
                                     ) != buildings.end()
                             );

        projectile->TryRemove(
                              find_if(
                                      enemies.begin(), enemies.end(),
                                      [&projectile](Enemy *enemy) -> bool {
                                          if (enemy->GetTank()->CollidesWith(projectile) && !enemy->IsDead()) {
                                              enemy->TakeDamage();
                                          }

                                          return enemy->GetTank()->CollidesWith(projectile);
                                      }
                                     ) != enemies.end()
                             );

        if (projectile->NeedsToBeRemoved()) {
            player->projectiles.erase(
                                      remove(player->projectiles.begin(), player->projectiles.end(), projectile), player->projectiles.end()
                                     );

            continue;
        }

        projectile->Move(deltaTimeSeconds);
    }
}

void Hw2::HandleEnemyProjectiles(Enemy *enemy, float deltaTimeSeconds) {
    for (auto &projectile: enemy->projectiles) {
        projectile->TryRemove(
                              find_if(
                                      buildings.begin(), buildings.end(),
                                      [&projectile](Building *building) -> bool {
                                          return building->CollidesWith(projectile);
                                      }
                                     ) != buildings.end()
                             );

        projectile->TryRemove(
                              [&projectile, this]() -> bool {
                                  if (player->GetTank()->CollidesWith(projectile)) {
                                      player->TakeDamage();
                                  }
                                  return player->GetTank()->CollidesWith(projectile);
                              }()
                             );

        if (projectile->NeedsToBeRemoved()) {
            enemy->projectiles.erase(
                                     remove(enemy->projectiles.begin(), enemy->projectiles.end(), projectile), enemy->projectiles.end()
                                    );

            continue;
        }

        projectile->Move(deltaTimeSeconds);
    }
}
