#pragma once

#include "../visuals/camera.h"
#include "../visuals/material.h"
#include "../../../components/simple_scene.h"
#include "../../../components/transform.h"

namespace custom {
    void RenderMesh(
        Mesh *mesh, custom::Material *material, custom::Camera *camera, Shader *shader, const glm::mat4 &modelMatrix,
        const glm::vec3 &lightPosition, const float health
        );
}; // namespace custom
