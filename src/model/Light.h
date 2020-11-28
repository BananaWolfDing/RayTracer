/**
  *  Author: Zhaobo Ding (me@dingzhaobo.net)
  */
#ifndef RAYTRACER_LIGHT_H
#define RAYTRACER_LIGHT_H


#include "../../glm/glm.hpp"

class Light {
public:
    glm::vec3 colour;
    glm::vec3 position;
    float falloff[3];
};


#endif //RAYTRACER_LIGHT_H
