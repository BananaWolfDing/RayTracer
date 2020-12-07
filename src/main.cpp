#include <fstream>
#include <iostream>
#include <glm/glm.hpp>
#include "RGB_Image.h"
#include "model/Scene.h"
#include "render/Tracer.h"
#include "model/Modeling.h"

int main() {
    std::cout << "Rendering!" << std::endl;

    Model test_model("examples/test.yml");

    std::vector<Object *> items;
    Scene *s = test_model.getScene(items);

    std::list<Light *> lights = test_model.getLights();
    Observer ob = test_model.getObserver();

    RGB_Image img(800, 800);

    std::cout << "Model loaded!" << std::endl;
    Tracer tracer(
            s,
            &img,
            ob.eye,
            ob.ambient,
            ob.up,
            ob.view,
            ob.fovy,
            lights
            );

    tracer.render();
    img.export_image("test.png");

    for (Object *obj : items) {
        delete obj;
    }
    for (Light *light : lights) {
        delete light;
    }
    delete s;
    return 0;
}
