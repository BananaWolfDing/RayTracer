/**
  *  Author: Zhaobo Ding (me@dingzhaobo.net)
  */
#include "Scene.h"
#include "Sphere.h"


HitRecord Scene::hit(Ray ray, float tmin, float tmax) {
    HitRecord res;

    if (leaf) {
        for (auto &obj : items) {
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

Scene::Scene() {
    leaf = true;
    Sphere *s = new Sphere(glm::vec3(), 10.0f);
    s->setMaterial(Material(
            glm::vec3(0, 0.467, 0.9),
            glm::vec3(0.8, 0.8, 0.8),
            25
            ));
    items.push_back(s);
    s = new Sphere(glm::vec3(-10.f, 0.f, -2.f), 3.0f);
    s->setMaterial(Material(
            glm::vec3(0, 0.9, 0.467),
            glm::vec3(0.8, 0.8, 0.8),
            25
            ));
    items.push_back(s);
    s = new Sphere(glm::vec3(10.f, 0.f, -2.f), 3.0f);
    s->setMaterial(Material(
            glm::vec3(0.9, 0.467, 0.0),
            glm::vec3(0.8, 0.8, 0.8),
            25
            ));
    items.push_back(s);

    s = new Sphere(glm::vec3(0.f, 0.f, -13.f), 2.0f);
    s->setMaterial(Material(
            glm::vec3(0.9, 0.0, 0.9),
            glm::vec3(0.8, 0.8, 0.8),
            25
            ));
    items.push_back(s);
    s = new Sphere(glm::vec3(0.f, 0.f, -17.f), 2.0f);
    s->setMaterial(Material(
            glm::vec3(0.9, 0.0, 0.9),
            glm::vec3(0.8, 0.8, 0.8),
            25
            ));
    items.push_back(s);
}

Scene::~Scene() {
    for (Object *obj : items) {
        delete(obj);
    }
}
