#pragma once

#include "utils/glm_utils.h"

namespace custom {
    class Material {
        float     shininess;
        float     Kd, Ks, Ke, Ka;
        glm::vec3 color;

    public:
        Material() : shininess(0), Kd(0), Ks(0), Ke(0), Ka(0), color(glm::vec3(1.0f)) {}

        Material(float shininess, float Kd, float Ks, float Ke, float Ka, glm::vec3 color) : shininess(shininess), Kd(Kd), Ks(Ks), Ke(Ke),
            Ka(Ka), color(color) {}

        Material(const Material &m) : shininess(m.shininess), Kd(m.Kd), Ks(m.Ks), Ka(m.Ka), Ke(m.Ke), color(m.color) {}

        ~Material() {}

        float     GetShininess() { return shininess; }
        float     GetKd() { return Kd; }
        float     GetKs() { return Ks; }
        float     GetKe() { return Ke; }
        float     GetKa() { return Ka; }
        glm::vec3 GetColor() { return color; }

        void SetShininess(float shininess) { this->shininess = shininess; }
        void SetKd(float Kd) { this->Kd = Kd; }
        void SetKs(float Ks) { this->Ks = Ks; }
        void SetKe(float Ke) { this->Ke = Ke; }
        void SetKa(float Ka) { this->Ka = Ka; }
        void SetColor(glm::vec3 color) { this->color = color; }
    };
}
