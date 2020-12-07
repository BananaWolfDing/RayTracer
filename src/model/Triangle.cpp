/**
  *  Author: Zhaobo Ding (me@dingzhaobo.net)
  */
#include "Triangle.h"

inline float min3(float x, float y, float z) {
    if (x < y && x < z) {
        return x;
    }
    else if (y < z) {
        return y;
    }
    else {
        return z;
    }
}

inline float max3(float x, float y, float z) {
    if (x > y && x > z) {
        return x;
    }
    else if (y > z) {
        return y;
    }
    else {
        return z;
    }
}

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

    if (abs(det) < 1e-3) {
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

bool Triangle::intersectBox(BoundingBox box) {
    /**  This method may return true even though a triangle
      *  does not intersect a bounding box
      */
    
    float x1 = min3(p1.x, p2.x, p3.x);
    float x2 = max3(p1.x, p2.x, p3.x);
    float y1 = min3(p1.y, p2.y, p3.y);
    float y2 = max3(p1.y, p2.y, p3.y);
    float z1 = min3(p1.z, p2.z, p3.z);
    float z2 = max3(p1.z, p2.z, p3.z);

    if (x1 > box.x2 || x2 < box.x1) {
        return false;
    }
    if (y1 > box.y2 || y2 < box.y1) {
        return false;
    }
    if (z1 > box.z2 || x2 < box.z1) {
        return false;
    }

    return true;
}