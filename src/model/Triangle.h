/**
  *  Author: Zhaobo Ding (me@dingzhaobo.net)
  */
#ifndef RAYTRACER_TRIANGLE_H
#define RAYTRACER_TRIANGLE_H


#include "../../glm/glm.hpp"
#include "../render/HitRecord.h"
#include "../render/Ray.h"
#include "Object.h"

class Triangle: Object {
    glm::vec3 p1, p2, p3;
public:
    Triangle();
    Triangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
    HitRecord hit(Ray ray, float tmin, float tmax) override;
};


#endif //RAYTRACER_TRIANGLE_H
