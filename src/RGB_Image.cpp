/**
  *  Author: Zhaobo Ding (me@dingzhaobo.net)
  */
#include <iostream>
#include "RGB_Image.h"
#include "../lodepng/lodepng.h"

RGB_Image::RGB_Image(uint32_t w, uint32_t h):
    width{w}, height{h}, img{std::vector(h, std::vector(w, glm::vec3()))} {

}

void RGB_Image::set_color(uint32_t x, uint32_t y, glm::vec3 color) {
    img[x][y] = color;
}

void RGB_Image::set_red(uint32_t x, uint32_t y, float red) {
    img[x][y].r = red;
}

void RGB_Image::set_green(uint32_t x, uint32_t y, float green) {
    img[x][y].g = green;
}

void RGB_Image::set_blue(uint32_t x, uint32_t y, float blue) {
    img[x][y].b = blue;
}

uint32_t RGB_Image::get_width() const {
    return width;
}

uint32_t RGB_Image::get_height() const {
    return height;
}

void RGB_Image::export_image(const std::string &filename) {
    std::vector<unsigned char> image((unsigned) width * height * 3);

    for (uint32_t i = 0; i < height; i++) {
        for (uint32_t j = 0; j < width; j++) {
            float r = glm::clamp(img[i][j].r, 0.0f, 1.0f);
            float g = glm::clamp(img[i][j].g, 0.0f, 1.0f);
            float b = glm::clamp(img[i][j].b, 0.0f, 1.0f);
            image[3 * ((i * width) + j) + 0] = (unsigned char) (255 * r);
            image[3 * ((i * width) + j) + 1] = (unsigned char) (255 * g);
            image[3 * ((i * width) + j) + 2] = (unsigned char) (255 * b);
        }
    }

    unsigned error = lodepng::encode(filename, image, width, height, LCT_RGB);
    if (error) {
        std::cout << "encoder error " << error << ": "<< lodepng_error_text(error) << std::endl;
    }
}