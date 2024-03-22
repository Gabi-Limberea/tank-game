#include "hw2.h"

#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "lab_m1/hw2/visuals/colors.h"
#include "utils/render.h"

using namespace m1;
using namespace std;

void Hw2::GenerateBuildings() {
    concrete = new custom::Material(0.2f, 0.2f, 0.2f, 0.0f, 0.55f, CONCRETE);

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> disBuildingCount(20, 30);

    int buildingCount = disBuildingCount(gen);

    for (int i = 0; i < buildingCount; i++) {
        Building *building = Building::GenerateNewBuilding(meshes["cube"], concrete, groundLevel, player->GetTank());

        buildings.push_back(building);
    }
}

void Hw2::GenerateEnemies() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> disEnemyCount(20, 30);

    int enemyCount = disEnemyCount(gen);

    for (int i = 0; i < enemyCount; i++) {
        Enemy *enemy = Enemy::GenerateEnemy(buildings, player->GetTank(), meshes, groundLevel);

        enemies.push_back(enemy);
    }
}

void Hw2::AddTankMeshes() {
    auto mesh = new Mesh("tank_body");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tank components"), "tank_body.obj");
    meshes[mesh->GetMeshID()] = mesh;

    mesh = new Mesh("tank_body_details_1");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tank components"), "tank_body_details_1.obj");
    meshes[mesh->GetMeshID()] = mesh;

    mesh = new Mesh("tank_body_details_2");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tank components"), "tank_body_details_2.obj");
    meshes[mesh->GetMeshID()] = mesh;

    mesh = new Mesh("tank_turret_details_1");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tank components"), "tank_turret_details_1.obj");
    meshes[mesh->GetMeshID()] = mesh;

    mesh = new Mesh("tank_cannon_details_1");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tank components"), "tank_cannon_details_1.obj");
    meshes[mesh->GetMeshID()] = mesh;

    mesh = new Mesh("tank_tracks");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tank components"), "tank_tracks.obj");
    meshes[mesh->GetMeshID()] = mesh;

    mesh = new Mesh("tank_tracks_connectors");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tank components"), "tank_tracks_connectors.obj");
    meshes[mesh->GetMeshID()] = mesh;

    mesh = new Mesh("tank_big_wheel");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tank components"), "tank_big_wheel.obj");
    meshes[mesh->GetMeshID()] = mesh;

    mesh = new Mesh("tank_small_wheel");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tank components"), "tank_small_wheel.obj");
    meshes[mesh->GetMeshID()] = mesh;

    mesh = new Mesh("tank_turret");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tank components"), "tank_turret.obj");
    meshes[mesh->GetMeshID()] = mesh;

    mesh = new Mesh("tank_cannon");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tank components"), "tank_cannon.obj");
    meshes[mesh->GetMeshID()] = mesh;

    mesh = new Mesh("tank_headlights");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tank components"), "tank_headlights.obj");
    meshes[mesh->GetMeshID()] = mesh;
}

void Hw2::InitShaders() {
    auto *shader = new Shader("BasicShader");
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "hw2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "hw2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
    shader->CreateAndLink();
    shaders[shader->GetName()] = shader;
}