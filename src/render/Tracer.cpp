/**
  *  Author: Zhaobo Ding (me@dingzhaobo.net)
  */
#include <thread>
#include <random>
#include <algorithm>
#include <cstdio>
#include <iostream>
#include "Tracer.h"

void progress_bar(float ratio) {
    uint8_t len = 60, end = int(len * ratio);

    std::cout << "[";
    for (uint8_t i = 0; i < len; i++) {
        if (i < end) {
            std::cout << "=";
        }
        else if (i == end) {
            std::cout << ">";
        }
        else {
            std::cout << " ";
        }
    }

    std::cout << "] " << int(ratio * 100) << " %\r";
    std::cout.flush();
}

Tracer::Tracer(
        Scene *const scene,
        RGB_Image *const img,
        const glm::vec3 eye,
        const glm::vec3 ambient,
        const glm::vec3 up,
        const glm::vec3 view,
        float fovy,
        const std::list<Light *> lights,
        Spacetime* spacetime,
        bool anti_aliasing,
        int nThreads):

        root{scene}, img{img}, eye{eye}, ambient{ambient},
        thread_row{0}, up{up}, fovy{fovy}, view{view},
        lights{lights}, h{img->get_height()}, w{img->get_width()},
        spacetime{spacetime},
        num_thread(nThreads)
{
    if (anti_aliasing) {
        num_sample = 10;
    }
    else {
        num_sample = 1;
    }

    secondary_limit = 20;
}

void Tracer::render() {
    thread_row = 0;
    std::vector<std::thread> threads;

    for (uint8_t i = 0; i < num_thread; i++) {
        threads.emplace_back(std::thread(&Tracer::Trace_thread, this));
    }

    while (thread_row <= h)
    {
			std::unique_lock<std::mutex> lock(progress_lock);
    	progress_cv.wait(lock);
    	progress_bar(progress);
    }

    for (auto &th: threads) {
        th.join();
    }
}

void Tracer::Trace_thread() {
    while (true) {
        progress = (float) thread_row / h;
        progress_cv.notify_all();
        uint32_t row = thread_row++;

        if (row >= h)
        	return;

        glm::vec3 front = glm::normalize(view);
        glm::vec3 above = glm::normalize(up);
        glm::vec3 right = glm::cross(front, above);
        float dist = (h / 2) / tan(glm::radians(fovy / 2));
        glm::vec3 base = dist * front + ((float) h / 2 - row) * above;
        for (uint32_t col = 0; col < w; col++) {
            glm::vec3 direction = base + ((float) w / 2 - col) * right;
						direction = glm::normalize(direction);

            if (num_sample == 1) {
                Ray ray(eye, direction);
                glm::vec3 color = trace(ray);
                img->set_color(row, col, color);
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

                img->set_color(row, col, color);
            }
        }
    }
}

bool Tracer::rayHit(Ray ray, HitRecord* const record)
{
	int const maxSteps = spacetime->getMaxSteps();
	float maxTime = spacetime->getInitialStepSize();
	float const infty = std::numeric_limits<float>::infinity();
	int steps = 0;

	while (!std::isnan(maxTime)
		&& root->get_box().contains(ray.getOrigin())
		&& steps < maxSteps
		)
	{
		// For some reason the max time here must be set to infinity.
    *record = root->hit(ray, steps == 0 ? 1e-3f : 0.f, infty);
    ++steps;

    if (record->isHit() && record->getTime() <= maxTime)
    	return true;

		if (std::isinf(maxTime))
			// Cannot hit anything despite infinite tolerance
			break;
		ray = spacetime->geodesic(ray, &maxTime);
		//maxTime = 1.0f;
  }
  return false;
}
glm::vec3 Tracer::trace(Ray ray, uint32_t secondary) {
	HitRecord record;
	if (!rayHit(ray, &record))
	{
		return glm::vec3(0.5f, 0.5f, 0.5f);
	}
  Material mat = record.getMaterial();
  glm::vec3 ds_color = diffuse_specular(ray, record);
  glm::vec3 color;
  if (secondary < secondary_limit) {
      glm::vec3 rr_color = reflect_refract(ray, record, secondary);
      color = glm::mix(ds_color, rr_color, mat.reflect() + mat.refract());
  }
  else {
      color = ds_color;
  }
  return color + mat.diffuse() * ambient;
}

glm::vec3 Tracer::diffuse_specular(Ray ray, HitRecord hitRecord) {
    glm::vec3 color = glm::vec3();
    for (Light *light: lights) {
        glm::vec3 l = glm::normalize(light->position - hitRecord.getPoint());
        Ray ray_out(hitRecord.getPoint(), l);
        HitRecord hit = root->hit(ray_out, 1e-3, std::numeric_limits<float>::max());

        // Warning: Do not enable spacetime tracing here unless we can solve
        // the direction problem
        //if (rayHit(ray_out, &hit)) {
        if (hit.isHit()) {
            float len_hit = glm::length(hit.getPoint() - hitRecord.getPoint());
            float len_light = glm::length(light->position - hitRecord.getPoint());
            if (len_hit < len_light) {
                continue;
            }
        }

        glm::vec3 n = hitRecord.getNormal();
        if (glm::dot(n, ray.getDirection()) > 0) {
            n = -n;
        }

        float specular_bright = 0;
        float diffuse_bright = std::max(0.0f, glm::dot(l, n));
        if (diffuse_bright > 0) {
            glm::vec3 r = glm::normalize(ray.getDirection() - 2 * glm::dot(ray.getDirection(), n) * n);
            glm::vec3 v = glm::normalize(eye - hitRecord.getPoint());
            specular_bright = pow(glm::dot(r, v), hitRecord.getMaterial().shininess());
        }

        float dist = glm::length(light->position - hitRecord.getPoint());
        glm::vec3 intensity = light->color /
                (float) (light->falloff[0] + light->falloff[1] * dist + light->falloff[2] * dist * dist);

        color += diffuse_bright * intensity * hitRecord.getMaterial().diffuse() +
                specular_bright * intensity * hitRecord.getMaterial().specular();
    }

    return color;
}

glm::vec3 Tracer::reflect_refract(Ray ray, HitRecord hitRecord, uint32_t secondary) {
    float refl = hitRecord.getMaterial().reflect();
    float refr = hitRecord.getMaterial().refract();

    if (refl > 0 || refr > 0) {
        glm::vec3 refl_color = glm::vec3(), refr_color = glm::vec3();

        glm::vec3 in_dirc = glm::normalize(ray.getDirection());
        glm::vec3 normal = hitRecord.getNormal();
        if (glm::dot(in_dirc, normal) > 0) {
            normal = -normal;
        }

        if (refr > 0) {
            if (hitRecord.getMaterial().transparent()) {
                Ray through_ray(hitRecord.getPoint(), ray.getDirection());
                refr_color = trace(through_ray, secondary + 1);
            }
            else {
                float ratio;
                if (glm::dot(in_dirc, hitRecord.getNormal()) < 0) {
                    ratio = 1 / refl;
                }
                else {
                    ratio = refl;
                }

                float cosine_in = glm::dot(in_dirc, hitRecord.getNormal());
                float sine_out2 = (ratio * ratio) * (1 - cosine_in * cosine_in);
                if (sine_out2 < 1) {
                    float cosine_out = sqrt(1.0 - sine_out2);
                    glm::vec3 refract_direction =
                        ratio * in_dirc
                        - ratio * glm::dot(in_dirc, normal) * normal
                        - cosine_out * normal;
                    Ray refract_ray(hitRecord.getPoint(), refract_direction);
                    refr_color = trace(refract_ray, secondary + 1);
                }
                else {
                    refl += refr; // total internal reflection
                }
            }
        }

        if (refl > 0) {
            glm::vec3 reflect_direction = ray.getDirection() -
                                          2 * glm::dot(ray.getDirection(), normal) * normal;
            Ray reflect_ray(hitRecord.getPoint(), reflect_direction);
            refl_color = trace(reflect_ray, secondary + 1);
        }

        return glm::mix(refl_color, refr_color, refr / (refl + refr));
    }
    else {
        return glm::vec3();
    }
}
