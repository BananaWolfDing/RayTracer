/**
  *  Author: Zhaobo Ding (me@dingzhaobo.net)
  */
#ifndef RAYTRACER_OBJECT_H
#define RAYTRACER_OBJECT_H

#include "../render/HitRecord.h"
#include "../render/Ray.h"
#include "BoundingBox.h"

class Object {
protected:
    Material mat;
public:
    void setMaterial(Material mat);

    virtual HitRecord hit(Ray ray, float tmin, float tmax) {
        return HitRecord();
    }

    virtual bool intersectBox(BoundingBox box) {
        return false;
    }
};

#endif //RAYTRACER_OBJECT_H
