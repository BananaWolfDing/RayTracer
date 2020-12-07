/**
  *  Author: Zhaobo Ding (me@dingzhaobo.net)
  */
#ifndef RAYTRACER_HITRECORD_H
#define RAYTRACER_HITRECORD_H

#include <glm/glm.hpp>
#include "../model/Material.h"

class HitRecord {
    bool hit;
    float hitTime;
    glm::vec3 normal, point;
    Material mat;
public:
    HitRecord();
    HitRecord(
      float t,
      glm::vec3 p,
      glm::vec3 n,
      glm::vec3 bump = glm::vec3(),
      Material mat = Material()
    );

    void setMaterial(Material mat);

    bool isHit() const;
    float getTime() const;
    glm::vec3 getNormal() const;
    glm::vec3 getPoint() const;
    Material getMaterial() const;
};

#endif //RAYTRACER_HITRECORD_H
