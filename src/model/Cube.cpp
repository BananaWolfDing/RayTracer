/**
  *  Author: Zhaobo Ding (me@dingzhaobo.net)
  */
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
            4, 5, 7,
            4, 6, 7,
            // back
            0, 1, 2,
            1, 2, 3,
            // left
            0, 2, 6,
            0, 4, 6,
            // right
            1, 3, 5,
            3, 5, 7,
            // top
            2, 3, 6,
            3, 6, 7,
            // bottom
            0, 1, 5,
            0, 4, 5
    };

    vertices[0] = p + e * glm::vec3(0, 0, 0);
    vertices[1] = p + e * glm::vec3(e, 0, 0);
    vertices[2] = p + e * glm::vec3(0, e, 0);
    vertices[3] = p + e * glm::vec3(e, e, 0);
    vertices[4] = p + e * glm::vec3(0, 0, e);
    vertices[5] = p + e * glm::vec3(e, 0, e);
    vertices[6] = p + e * glm::vec3(0, e, e);
    vertices[7] = p + e * glm::vec3(e, e, e);

    for (uint8_t i = 0; i < 12; i++) {
        glm::vec3 p1 = vertices[i * 3 + 0];
        glm::vec3 p2 = vertices[i * 3 + 1];
        glm::vec3 p3 = vertices[i * 3 + 2];

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

    return res;
}