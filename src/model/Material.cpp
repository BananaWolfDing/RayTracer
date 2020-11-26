/**
  *  Author: Zhaobo Ding (me@dingzhaobo.net)
  */
#include "Material.h"

Material::Material() {}

Material::Material(glm::vec3 kd, glm::vec3 ks, double shine, double refl, double refr):
    kd{kd}, ks{ks}, shine{shine}, refl{refl}, refr{refr} {}

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