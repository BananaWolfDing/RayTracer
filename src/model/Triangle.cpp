/**
  *  Author: Zhaobo Ding (me@dingzhaobo.net)
  */
#include "Triangle.h"

Triangle::Triangle(): p1{}, p2{}, p3{} {}

Triangle::Triangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3):
    p1{p1}, p2{p2}, p3{p3} {}

HitRecord Triangle::hit(Ray ray, float tmin, float tmax) {
    glm::vec3 T = ray.getOrigin() - p1;
    glm::vec3 V1 = p2 - p1;
    glm::vec3 V2 = p3 - p1;
    glm::vec3 D = ray.getDirection();

    glm::vec3 P = glm::cross(V1, D);
    float det = glm::dot(P, V2);

    if (det == 0) {
        return HitRecord();
    }

    float inv = 1 / det;

    glm::vec3 Q = glm::cross(V2, T);
    float x = inv * glm::dot(Q, D);
    float y = inv * glm::dot(P, T);

    if (x < 0 || x > 1 || y < 0 || x + y > 1) {
        return HitRecord();
    }

    float t = inv * glm::dot(Q, V1);
    return HitRecord(
                t,
                ray.at(t),
                glm::cross(V1, V2),
                this->mat
                );
}