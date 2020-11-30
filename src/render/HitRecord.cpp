/**
  *  Author: Zhaobo Ding (me@dingzhaobo.net)
  */
#include "HitRecord.h"

HitRecord::HitRecord(): hit{false} {}

HitRecord::HitRecord(float t, glm::vec3 p, glm::vec3 n, Material mat):
    hit{true}, hitTime{t}, point{p}, mat{mat}
{
    normal = glm::normalize(n);
}

void HitRecord::setMaterial(Material mat) {
    this->mat = mat;
}

bool HitRecord::isHit() const {
    return hit;
}

float HitRecord::getTime() const {
    return hitTime;
}

glm::vec3 HitRecord::getPoint() const {
    return point;
}

glm::vec3 HitRecord::getNormal() const {
    return normal;
}

Material HitRecord::getMaterial() const {
    return mat;
}