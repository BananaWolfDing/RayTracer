/**
  *  Author: Zhaobo Ding (me@dingzhaobo.net)
  */
#ifndef RAYTRACER_OBJECT_H
#define RAYTRACER_OBJECT_H

#include "../render/HitRecord.h"
#include "../render/Ray.h"
#include "bump/BumpMapping.h"
#include "BoundingBox.h"

class Object {
protected:
    Material mat;
    mapping *bump_mapping = &plain_mapping;
public:
    void setMaterial(Material mat);
    void setMapping(mapping *map);

    virtual HitRecord hit(Ray ray, float tmin, float tmax) {
        return HitRecord();
    }

    virtual bool intersectBox(BoundingBox box) {
        return false;
    }
};

#endif //RAYTRACER_OBJECT_H
