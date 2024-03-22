#pragma once

#pragma once

#include "utils/glm_utils.h"
#include "utils/math_utils.h"


namespace custom {
    class Camera {
    public:
        float distanceToTarget;
        float fov;
        float aspectRatio;
        float zNear, zFar;
        glm::vec3 position;
        glm::vec3 forward;
        glm::vec3 right;
        glm::vec3 up;

        Camera() : position(glm::vec3(0, 2, 5)), forward(glm::vec3(0, 0, -1)), up(glm::vec3(0, 1, 0)),
                   right(glm::vec3(1, 0, 0)), distanceToTarget(3), fov(60 * TO_RADIANS), aspectRatio(16.0f / 9.0f), zNear(0.01f),
                   zFar(200.0f) {}

        Camera(
            const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up, const float distanceToTarget,
            const float fov, const float aspectRatio, const float zNear, const float zFar
            ) {
            this->position = position;
            forward = glm::normalize(center - position);
            right = glm::cross(forward, up);
            this->up = glm::cross(right, forward);
            this->distanceToTarget = distanceToTarget;
            this->fov = fov;
            this->aspectRatio = aspectRatio;
            this->zNear = zNear;
            this->zFar = zFar;
        }

        ~Camera() {}

        void MoveForward(float distance);
        void TranslateForward(float distance);
        void TranslateUpward(float distance);
        void TranslateRight(float distance);

        void RotateFirstPerson_OX(float angle);
        void RotateFirstPerson_OY(float angle);
        void RotateFirstPerson_OZ(float angle);

        void RotateThirdPerson_OX(float angle);
        void RotateThirdPerson_OY(float angle);
        void RotateThirdPerson_OZ(float angle);

        glm::mat4 GetViewMatrix();
        glm::vec3 GetTargetPosition();
        glm::mat4 GetProjectionMatrix();
    };
} // namespace custom
