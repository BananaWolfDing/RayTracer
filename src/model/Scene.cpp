/**
  *  Author: Zhaobo Ding (me@dingzhaobo.net)
  */
#include "Scene.h"

HitRecord Scene::hit(Ray ray, float tmin, float tmax) {
    HitRecord res;

    if (leaf) {
        for (auto &obj : items) {
            HitRecord hit = obj.hit(ray, tmin, tmax);
            if (hit.isHit() && hit.getTime() >= tmin && hit.getTime() <= tmax) {
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
    }
    else {
        if (box.hitBox(ray, tmin, tmax)) {
            for (auto &child : children) {
                HitRecord hit = child.hit(ray, tmin, tmax);
                if (hit.isHit() && hit.getTime() >= tmin && hit.getTime() <= tmax) {
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
        }
        else {
            return HitRecord();
        }
    }

    return res;
}