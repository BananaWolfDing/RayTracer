/**
  *  Author: Zhaobo Ding (me@dingzhaobo.net)
  */
#ifndef RAYTRACER_BOUNDINGBOX_H
#define RAYTRACER_BOUNDINGBOX_H


#include "../render/Ray.h"

class BoundingBox {
    float x1, x2, y1, y2, z1, z2;
public:
    BoundingBox();
    BoundingBox(float x1, float x2, float y1, float y2, float z1, float z2);
    bool hitBox(Ray ray, float tmin, float tmax);
};


#endif //RAYTRACER_BOUNDINGBOX_H
