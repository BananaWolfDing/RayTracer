/**
  *  Author: Zhaobo Ding (me@dingzhaobo.net)
  */
#include "Material.h"

Material::Material() {}

Material::Material(
        glm::vec3 kd,
        glm::vec3 ks,
        double shine,
        double refl,
        double refr,
        bool tran):
    kd{kd}, ks{ks}, shine{shine}, refl{refl},
    refr{refr}, transparency(tran) {}

Material& Material::operator=(const Material &other) {
    if (&other == this) {
        return *this;
    }

    this->kd = other.kd;
    this->ks = other.ks;
    this->refl = other.refl;
    this->refr = other.refr;
    this->shine = other.shine;

    return *this;
}

glm::vec3 Material::diffuse() const {
    return kd;
}

glm::vec3 Material::specular() const {
    return ks;
}

double Material::shininess() const {
    return shine;
}

double Material::reflect() const {
    return refl;
}

double Material::refract() const {
    return refr;
}

bool Material::transparent() const {
    return transparency;
}