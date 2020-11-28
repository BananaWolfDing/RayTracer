/**
  *  Author: Zhaobo Ding (me@dingzhaobo.net)
  */
#ifndef RAYTRACER_TRACER_H
#define RAYTRACER_TRACER_H

#include <cstdint>
#include <list>
#include <atomic>
#include "../model/Scene.h"
#include "../RGB_Image.h"
#include "../model/Light.h"


class Tracer {
    Scene *root;
    RGB_Image *img;
    const glm::vec3 eye;
    const glm::vec3 ambient;
    const glm::vec3 up;
    const glm::vec3 view;
    const float fovy;
    const std::list<Light *> lights;

    const size_t h, w;
    uint32_t num_thread, num_sample;
    std::atomic_uint thread_row;

    void Trace_thread();
    glm::vec3 trace(Ray ray);
    glm::vec3 diffuse_specular(Ray ray, HitRecord hitRecord);
public:
    Tracer(Scene *scene,
            RGB_Image *img,
            glm::vec3 eye,
            glm::vec3 ambient,
            glm::vec3 up,
            glm::vec3 view,
            float fovy,
            std::list<Light *> lights,
            bool anti_aliasing = false,
            bool multi_thread = false);
    void render();
};


#endif //RAYTRACER_TRACER_H
