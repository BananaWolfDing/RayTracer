/**
  *  Author: Zhaobo Ding (me@dingzhaobo.net)
  */
#ifndef RAYTRACER_CUBE_H
#define RAYTRACER_CUBE_H

#include <vector>
#include "Object.h"
#include "Triangle.h"

class Cube: public Object {
    glm::vec3 pos;
    float edge;
    glm::vec3 vertices[8];
    Triangle tri[12];
public:
    Cube(glm::vec3 p, float e);
    HitRecord hit(Ray ray, float tmin, float tmax) override;
};


#endif //RAYTRACER_CUBE_H
