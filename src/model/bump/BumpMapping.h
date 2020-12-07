/**
  *  Author: Zhaobo Ding (me@dingzhaobo.net)
  */
#ifndef RAYTRACER_BUMPMAPPING_H
#define RAYTRACER_BUMPMAPPING_H

#include <glm/glm.hpp>

typedef glm::vec3 mapping(glm::vec3);

glm::vec3 plain_mapping(glm::vec3 pos);

glm::vec3 floor_mapping(glm::vec3 pos);

#endif //RAYTRACER_BUMPMAPPING_H
