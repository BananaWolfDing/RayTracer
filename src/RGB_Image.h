/**
  *  Author: Zhaobo Ding (me@dingzhaobo.net)
  */
#ifndef RAYTRACER_RGB_IMAGE_H
#define RAYTRACER_RGB_IMAGE_H


#include <vector>
#include <string>
#include "../glm/glm.hpp"

class RGB_Image {
    uint32_t width, height;
    std::vector<std::vector<glm::vec3>> img;

public:
    RGB_Image(uint32_t w, uint32_t h);

    void set_color(uint32_t x, uint32_t y, glm::vec3 color);
    void set_red(uint32_t x, uint32_t y, float red);
    void set_green(uint32_t x, uint32_t y, float green);
    void set_blue(uint32_t x, uint32_t y, float blue);

    uint32_t get_width() const;
    uint32_t get_height() const;

    void export_image(const std::string &filename);
};


#endif //RAYTRACER_RGB_IMAGE_H
