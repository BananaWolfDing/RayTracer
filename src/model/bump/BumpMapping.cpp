/**
  *  Author: Zhaobo Ding (me@dingzhaobo.net)
  */
#include <cmath>
#include "BumpMapping.h"

glm::vec3 plain_mapping(glm::vec3 pos) {
    return glm::vec3();
}

glm::vec3 floor_mapping(glm::vec3 pos) {
    float modx = pos.x - floor(pos.x / 10) * 10;
    float modz = pos.z - floor(pos.z / 10) * 10;
    if ((modx - 5) * (modx - 5) + (modz - 5) * (modz - 5) <= 1) {
        glm::vec3 point(
            modx,
            -sqrt(1 - (modx - 5) * (modx - 5) - (modz - 5) * (modz - 5)),
            modz
        );
        return 0.2f * (glm::vec3(modx, 0, modz) - point);
    }
    else {
        return glm::vec3();
    }
}