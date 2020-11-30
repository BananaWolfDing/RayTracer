/**
  *  Author: Zhaobo Ding (me@dingzhaobo.net)
  */
#include "Light.h"

Light::Light(glm::vec3 color, glm::vec3 pos, float *falloff):
    color{color}, position{pos} {

    this->falloff[0] = falloff[0];
    this->falloff[1] = falloff[1];
    this->falloff[2] = falloff[2];
}