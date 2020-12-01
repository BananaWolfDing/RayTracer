/**
  *  Author: Zhaobo Ding (me@dingzhaobo.net)
  */
#ifndef RAYTRACER_MATERIAL_H
#define RAYTRACER_MATERIAL_H


#include <glm/glm.hpp>

class Material {
    bool transparency;
    double shine, refl, refr;
    glm::vec3 kd, ks;

public:
    Material();
    Material(
            glm::vec3 kd,
            glm::vec3 ks,
            double shine,
            double refl = 0,
            double refr = 0,
            bool tran = false);

    Material &operator=(const Material &other);

    glm::vec3 diffuse() const;
    glm::vec3 specular() const;

    double shininess() const;
    double reflect() const;
    double refract() const;
    bool transparent() const;
};


#endif //RAYTRACER_MATERIAL_H
