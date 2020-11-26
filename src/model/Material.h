/**
  *  Author: Zhaobo Ding (me@dingzhaobo.net)
  */
#ifndef RAYTRACER_MATERIAL_H
#define RAYTRACER_MATERIAL_H


#include "../../glm/glm.hpp"

class Material {
    double shine, refl, refr;
    glm::vec3 kd, ks;

public:
    Material();
    Material(glm::vec3 kd, glm::vec3 ks, double shine, double refl = 0, double refr = 0);

    glm::vec3 diffuse() const;
    glm::vec3 specular() const;

    double shininess() const;
    double reflect() const;
    double refract() const;
};


#endif //RAYTRACER_MATERIAL_H
