/**
  *  Author: Zhaobo Ding (me@dingzhaobo.net)
  */
#include "Sphere.h"
#include <cmath>


struct quadraticSolution {
    uint32_t num;
    float smaller, greater;

    explicit quadraticSolution(uint32_t n, float r1 = 0, float r2 = 0) {
        num = n;
        switch (n) {
            case 1:
                smaller = r1;
                break;
            case 2:
                if (r1 > r2) {
                    smaller = r2;
                    greater = r1;
                }
                else {
                    smaller = r1;
                    greater = r2;
                }
                break;
        }
    }
};

quadraticSolution quadraticRoots(float A, float B, float C) {
    if (A == 0) {
        if (B == 0) {
            return quadraticSolution(0);
        }
        else {
            return quadraticSolution(1, -C / B);
        }
    }
    else {
        float Delta = B * B - 4 * A * C;

        if (Delta < 0) {
            return quadraticSolution(0);
        }
        else if (Delta == 0) {
            return quadraticSolution(1, -B / (A * 2));
        }
        else {
            float r1 = (-B + sqrt(Delta)) / (A * 2);
            float r2 = (-B - sqrt(Delta)) / (A * 2);
            return quadraticSolution(2, r1, r2);
        }
    }
}

Sphere::Sphere(glm::vec3 p, float r): pos{p}, radius{r} {}

HitRecord Sphere::hit(Ray ray, float tmin, float tmax) {
    /*
     *  / p = origin + t*direction
     *  |
     *  \ (p - center)^2 = radius^2
     *
     *  The system above can be simplified to
     *  direction^2 * t^2 + 2 * (origin - center) * direction * t + (origin - center)^2 - radius^2 = 0
     */

    float A = glm::dot(ray.getDirection(), ray.getDirection());
    float B = glm::dot(ray.getDirection(), ray.getOrigin() - pos) * 2;
    float C = glm::dot(ray.getOrigin() - pos, ray.getOrigin() - pos) - radius * radius;

    quadraticSolution sol = quadraticRoots(A, B, C);

    switch (sol.num) {
        case 0:
            return HitRecord();
        case 1:
            if (sol.smaller >= tmin && sol.smaller <= tmax) {
                return HitRecord(
                        sol.smaller,
                        ray.at(sol.smaller),
                        ray.at(sol.smaller) - pos,
                        this->mat
                        );
            }
            else {
                return HitRecord();
            }
        case 2:
            if (sol.smaller >= tmin && sol.smaller <= tmax) {
                return HitRecord(
                        sol.smaller,
                        ray.at(sol.smaller),
                        ray.at(sol.smaller) - pos,
                        this->mat
                        );
            }
            else if (sol.greater >= tmin && sol.greater <= tmax) {
                return HitRecord(
                        sol.greater,
                        ray.at(sol.greater),
                        ray.at(sol.greater) - pos,
                        this->mat
                        );
            }
            else {
                return HitRecord();
            }
        default:
            return HitRecord();
    }
}