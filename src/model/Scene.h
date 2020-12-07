/**
  *  Author: Zhaobo Ding (me@dingzhaobo.net)
  */
#ifndef RAYTRACER_SCENE_H
#define RAYTRACER_SCENE_H

#include <vector>
#include <cstdio>
#include "BoundingBox.h"
#include "Object.h"

class Scene {
    bool leaf;
    BoundingBox box;
    std::vector<Scene *> children;
    std::vector<Object *> items;
public:
    Scene();
    Scene(
      std::vector<Object *> objects,
      float x1,
      float x2,
      float y1,
      float y2,
      float z1,
      float z2,
      uint8_t depth);
    ~Scene();
    HitRecord hit(Ray ray, float tmin, float tmax);

    BoundingBox const& get_box() const { return box; }
};


#endif //RAYTRACER_SCENE_H
