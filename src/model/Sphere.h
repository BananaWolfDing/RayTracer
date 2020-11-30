/**
  *  Author: Zhaobo Ding (me@dingzhaobo.net)
  */
#ifndef RAYTRACER_SPHERE_H
#define RAYTRACER_SPHERE_H


#include "Object.h"

class Sphere: public Object {
    glm::vec3 pos;
    float radius;
public:
    Sphere(glm::vec3 p, float r);
    HitRecord hit(Ray ray, float tmin, float tmax) override;
};


#endif //RAYTRACER_SPHERE_H
