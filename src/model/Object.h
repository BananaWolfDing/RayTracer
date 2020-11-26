/**
  *  Author: Zhaobo Ding (me@dingzhaobo.net)
  */
#ifndef RAYTRACER_OBJECT_H
#define RAYTRACER_OBJECT_H

#include "../render/HitRecord.h"
#include "../render/Ray.h"

class Object {
public:
    virtual HitRecord hit(Ray ray, float tmin, float tmax);
};

#endif //RAYTRACER_OBJECT_H
