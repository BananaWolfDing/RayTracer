/**
  *  Author: Zhaobo Ding (me@dingzhaobo.net)
  */
#include <thread>
#include <random>
#include "Tracer.h"

Tracer::Tracer(
        Scene *const scene,
        RGB_Image *const img,
        const glm::vec3 eye,
        const glm::vec3 ambient,
        const glm::vec3 up,
        const glm::vec3 view,
        float fovy,
        const std::list<Light *> lights,
        bool anti_aliasing,
        bool multi_thread):

        root{scene}, img{img}, eye{eye}, ambient{ambient},
        thread_row{0}, up{up}, fovy{fovy}, view{view},
        lights{lights}, h{img->get_height()}, w{img->get_width()} {

    if (multi_thread) {
        num_thread = 50;
    }
    else {
        num_thread = 1;
    }

    if (anti_aliasing) {
        num_sample = 10;
    }
    else {
        num_sample = 1;
    }
}

void Tracer::render() {
    thread_row = 0;
    std::vector<std::thread> threads;

    for (uint8_t i = 0; i < num_thread; i++) {
        threads.emplace_back(std::thread(&Tracer::Trace_thread));
    }

    for (auto &th: threads) {
        th.join();
    }
}

void Tracer::Trace_thread() {
    while (true) {
        uint32_t row = thread_row++;
        if (row >= h) {
            return;
        }

        glm::vec3 front = glm::normalize(view);
        glm::vec3 above = glm::normalize(up);
        glm::vec3 right = glm::cross(front, above);
        float dist = (h / 2) / tan(glm::radians(fovy / 2));
        glm::vec3 base = dist * front + ((float) h / 2 - row) * above;
        for (uint32_t col = 0; col < w; col++) {
            glm::vec3 direction = base + ((float) w / 2 - col) * right;

            if (num_sample == 1) {
                Ray ray(eye, direction);
                glm::vec3 color = trace(ray);
                img->set_color(thread_row, col, color);
            }
            else {
                glm::vec3 color;
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_real_distribution<> dis(0, 0.5);

                for (uint32_t i = 0; i < num_sample; i++) {
                    Ray ray(eye,
                            direction +
                            (float) dis(gen) * right +
                            (float) dis(gen) * above);
                    color += trace(ray) / (float) num_sample;
                }

                img->set_color(thread_row, col, color);
            }
        }
    }
}