/**
  *  Author: Zhaobo Ding (me@dingzhaobo.net)
  */
#include "Ray.h"

glm::vec3 Ray::getOrigin() const {
    return orig;
}

glm::vec3 Ray::getDirection() const {
    return dirc;
}

glm::vec3 Ray::at(float t) {
    return orig + dirc * t;
}