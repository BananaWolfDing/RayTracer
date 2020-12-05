#include <fstream>
#include <iostream>
#include <glm/glm.hpp>
#include "RGB_Image.h"
#include "model/Scene.h"
#include "render/Tracer.h"
#include "model/Modeling.h"

int main() {
    std::cout << "Rendering!" << std::endl;

    Model test_model("models/test.yml");
    Scene *s = test_model.getScene();
    test_model.getLights();
    test_model.getObserver();
    delete s;
    // RGB_Image img(800, 800);
    // Scene scene = Scene();

    // float fall[3] = {1, 0, 0};
    // Light light(glm::vec3(0.8, 0.8, 0.8), glm::vec3(100, 100, 100), fall);
    // std::list<Light *> lights;
    // lights.emplace_back(&light);

    // Tracer tracer(
    //         &scene,
    //         &img,
    //         glm::vec3(0, 0, 100),
    //         glm::vec3(0, 0, 0),
    //         glm::vec3(0, 1, 0),
    //         glm::vec3(0, 0, -1),
    //         50.0f,
    //         lights
    //         );

    // tracer.render();
    // img.export_image("test.png");
    return 0;
}