/**
  *  Author: Zhaobo Ding (me@dingzhaobo.net)
  */
#include "Scene.h"
#include "Sphere.h"
#include <iostream>

HitRecord Scene::hit(Ray ray, float tmin, float tmax) {
    HitRecord res;

    if (leaf) {
        for (Object *obj : items) {
            HitRecord hit = obj->hit(ray, tmin, tmax);
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
            for (Scene *child : children) {
                HitRecord hit = child->hit(ray, tmin, tmax);
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

Scene::Scene() {
    leaf = true;
    Sphere *s = new Sphere(glm::vec3(), 10.0f);
    s->setMaterial(Material(
            glm::vec3(0, 0.467, 0.9),
            glm::vec3(0.8, 0.8, 0.8),
            25
            ));
    items.push_back(s);
}

Scene::Scene(
      std::vector<Object *> objects,
      float x1,
      float x2,
      float y1,
      float y2,
      float z1,
      float z2,
      uint8_t depth) {
    
    uint32_t len = objects.size();
    box = BoundingBox(x1, x2, y1, y2, z1, z2);

    if (depth > 0 && len > 1) {
        leaf = false;
        float dx = abs(x1 - x2);
        float dy = abs(y1 - y2);
        float dz = abs(z1 - z2);

        if (dx >= dy && dx >= dz) {
            float mid = (x1 + x2) / 2;
            BoundingBox b1(x1, mid, y1, y2, z1, z2);
            BoundingBox b2(mid, x2, y1, y2, z1, z2);
            std::vector<Object *> o1, o2;

            for (Object *item : objects) {
                if (item->intersectBox(b1)) {
                    o1.push_back(item);
                }
                if (item->intersectBox(b2)) {
                    o2.push_back(item);
                }
            }

            children.push_back(new Scene(
                o1, x1, mid, y1, y2, z1, z2, depth - 1
            ));
            children.push_back(new Scene(
                o2, mid, x2, y1, y2, z1, z2, depth - 1
            ));
        }
        else if (dy >= dz) {
            float mid = (y1 + y2) / 2;
            BoundingBox b1(x1, x2, y1, mid, z1, z2);
            BoundingBox b2(x1, x2, mid, y2, z1, z2);
            std::vector<Object *> o1, o2;

            for (Object *item : objects) {
                if (item->intersectBox(b1)) {
                    o1.push_back(item);
                }
                if (item->intersectBox(b2)) {
                    o2.push_back(item);
                }
            }

            children.push_back(new Scene(
                o1, x1, x2, y1, mid, z1, z2, depth - 1
            ));
            children.push_back(new Scene(
                o2, x1, x2, mid, y2, z1, z2, depth - 1
            ));
        }
        else {
            float mid = (z1 + z2) / 2;
            BoundingBox b1(x1, x2, y1, y2, z1, mid);
            BoundingBox b2(x1, x2, y1, y2, mid, z2);
            std::vector<Object *> o1, o2;

            for (Object *item : objects) {
                if (item->intersectBox(b1)) {
                    o1.push_back(item);
                }
                if (item->intersectBox(b2)) {
                    o2.push_back(item);
                }
            }

            children.push_back(new Scene(
                o1, x1, x2, y1, y2, z1, mid, depth - 1
            ));
            children.push_back(new Scene(
                o2, x1, x2, y1, y2, mid, z2, depth - 1
            ));
        }
    }
    else {
        leaf = true;
        for (Object *item : objects) {
            items.push_back(item);
        }
    }
}

Scene::~Scene() {
    for (Scene *s : children) {
        delete s;
    }
}