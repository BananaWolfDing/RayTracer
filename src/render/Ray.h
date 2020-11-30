/**
  *  Author: Zhaobo Ding (me@dingzhaobo.net)
  */
#ifndef RAYTRACER_RAY_H
#define RAYTRACER_RAY_H


#include <glm/glm.hpp>

class Ray {
    glm::vec3 orig, dirc;
public:
    Ray(glm::vec3 orig, glm::vec3 dirc);
    glm::vec3 getOrigin() const;
    glm::vec3 getDirection() const;
    glm::vec3 at(float t);
};


#endif //RAYTRACER_RAY_H
