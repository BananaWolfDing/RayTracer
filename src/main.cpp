#include <iostream>
#include "RGB_Image.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    RGB_Image img(800, 800);

    for (uint32_t i = 0; i < 800; i++)
        for (uint32_t j = 0; j < 800; j++)
            img.set_color(i, j, glm::vec3(0.5, 0.5, 0.5));

    img.export_image("test.png");

    return 0;
}