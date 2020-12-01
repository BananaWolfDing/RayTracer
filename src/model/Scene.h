/**
  *  Author: Zhaobo Ding (me@dingzhaobo.net)
  */
#ifndef RAYTRACER_SCENE_H
#define RAYTRACER_SCENE_H

#include <vector>
#include "BoundingBox.h"
#include "Object.h"

class Scene {
    bool leaf;
    BoundingBox box;
    std::vector<Scene> children;
    std::vector<Object *> items;
public:
    Scene();
    ~Scene();
    HitRecord hit(Ray ray, float tmin, float tmax);

    BoundingBox const& get_box() const { return box; }
};


#endif //RAYTRACER_SCENE_H
