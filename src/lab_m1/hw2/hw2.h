#pragma once

#include <vector>

#include "enemy.h"
#include "player.h"
#include "components/simple_scene.h"
#include "components/text_renderer.h"
#include "components/transform.h"
#include "lab_m1/hw2/objects/building.h"
#include "visuals/camera.h"


namespace m1 {
    class Hw2 : public gfxc::SimpleScene {
    public:
        Hw2();
        ~Hw2();

        void Init() override;

    private:
        custom::Camera *camera;
        gfxc::TextRenderer *textRenderer;

        glm::vec3 lightPosition;
        glm::vec3 groundLevel;

        Player *player;
        std::vector<Building *> buildings;
        std::vector<Enemy *> enemies;

        float timer;
        int deadEnemies;
        custom::Material *dirt;
        custom::Material *concrete;

        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        void AddTankMeshes();
        void InitShaders();
        void HandleEnemies(float deltaTimeSeconds);
        void GenerateBuildings();
        void GenerateEnemies();
        void RenderBuildings();
        void RenderEnemies();
        void Hw2::HandlePlayerProjectiles(float deltaTimeSeconds);
        void Hw2::HandleEnemyProjectiles(Enemy *enemy, float deltaTimeSeconds);
    };
} // namespace m1
