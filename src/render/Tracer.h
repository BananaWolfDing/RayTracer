/**
  *  Author: Zhaobo Ding (me@dingzhaobo.net)
  */
#ifndef RAYTRACER_TRACER_H
#define RAYTRACER_TRACER_H

#include <cstdint>
#include <list>
#include <atomic>
#include <mutex>
#include <condition_variable>

#include "../spacetime/Spacetime.h"
#include "../model/Scene.h"
#include "../model/Light.h"
#include "../RGB_Image.h"


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
		Spacetime* spacetime;

    uint32_t num_thread, num_sample;
    uint32_t secondary_limit;
    std::atomic_uint thread_row;

		std::atomic<float> progress;
		std::mutex progress_lock;
		std::condition_variable progress_cv;

    void Trace_thread();
    bool rayHit(Ray ray, HitRecord* const hitRecord);
    glm::vec3 trace(Ray ray, uint32_t secondary = 0);
    glm::vec3 diffuse_specular(Ray ray, HitRecord hitRecord);
    glm::vec3 reflect_refract(Ray ray, HitRecord hitRecord, uint32_t secondary = 0);
public:
    Tracer(Scene *scene,
            RGB_Image *img,
            glm::vec3 eye,
            glm::vec3 ambient,
            glm::vec3 up,
            glm::vec3 view,
            float fovy,
            std::list<Light *> lights,
            Spacetime* spacetime,
            bool anti_aliasing = false,
            int nThreads = 1);

    void render();
};


#endif //RAYTRACER_TRACER_H
