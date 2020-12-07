/**
  *  Author: Zhaobo Ding (me@dingzhaobo.net)
  */
#include <algorithm>
#include "Cube.h"

Cube::Cube(glm::vec3 p, float e): pos{p}, edge{e} {
    /*
     *  2------3
     *  |\     |\
     *  | 6------7
     *  | |    | |
     *  0-|----1 |
     *   \|     \|
     *    4------5
	*/

    uint8_t triangles[36] = {
            // front
            5, 7, 4,
            6, 4, 7,
            // back
            0, 2, 1,
            3, 1, 2,
            // left
            2, 0, 6,
            4, 6, 0,
            // right
            1, 3, 5,
            7, 5, 3,
            // top
            2, 6, 3,
            7, 3, 6,
            // bottom
            1, 5, 0,
            4, 0, 5
    };

    vertices[0] = p + e * glm::vec3(0, 0, 0);
    vertices[1] = p + e * glm::vec3(0, 0, 1);
    vertices[2] = p + e * glm::vec3(0, 1, 0);
    vertices[3] = p + e * glm::vec3(0, 1, 1);
    vertices[4] = p + e * glm::vec3(1, 0, 0);
    vertices[5] = p + e * glm::vec3(1, 0, 1);
    vertices[6] = p + e * glm::vec3(1, 1, 0);
    vertices[7] = p + e * glm::vec3(1, 1, 1);

    for (uint8_t i = 0; i < 12; i++) {
        glm::vec3 p1 = vertices[triangles[i * 3 + 0]];
        glm::vec3 p2 = vertices[triangles[i * 3 + 1]];
        glm::vec3 p3 = vertices[triangles[i * 3 + 2]];

        tri[i] = Triangle(p1, p2, p3);
    }
}

HitRecord Cube::hit(Ray ray, float tmin, float tmax) {
    HitRecord res;
    for (auto &triangle : tri) {
        HitRecord hit = triangle.hit(ray, tmin, tmax);

        if (hit.isHit()) {
            if (res.isHit()) {
                if (res.getTime() > hit.getTime()) {
                    res = hit;
                }
            }
            else {
                res = hit;
            }
        }
    }

    res.setMaterial(this->mat);
    return res;
}

bool Cube::intersectBox(BoundingBox box) {
    float x1 = vertices[0].x, x2 = vertices[0].x;
    float y1 = vertices[0].y, y2 = vertices[0].y;
    float z1 = vertices[0].z, z2 = vertices[0].z;
    
    for (auto &p : vertices) {
        x1 = std::min(x1, p.x);
        x2 = std::max(x2, p.x);
        y1 = std::min(y1, p.y);
        y2 = std::max(y2, p.y);
        z1 = std::min(z1, p.z);
        z2 = std::max(z2, p.z);
    }

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