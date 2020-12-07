/**
  *  Author: Zhaobo Ding (me@dingzhaobo.net)
  */
#ifndef RAYTRACER_MODELING_H
#define RAYTRACER_MODELING_H

#include <string>
#include <glm/glm.hpp>
#include "Scene.h"
#include "Light.h"
#include "yaml-cpp/yaml.h"


struct ModelingException : public std::exception {
  std::string e;
  ModelingException(std::string s): e(s) {}
  const char *what() const throw() {
    return e.c_str();
  }
};

struct Observer {
  glm::vec3 eye, ambient, up, view;
  float fovy;

  Observer(
    glm::vec3 eye,
    glm::vec3 ambient,
    glm::vec3 up,
    glm::vec3 view,
    float fovy):

    eye{eye}, ambient{ambient}, up{up}, view{view}, fovy{fovy} {}
};

class Model {
  YAML::Node info;
public:
  Model(const std::string &filename);

  Scene *getScene(std::vector<Object *> &objList) const;
  std::list<Light *> getLights() const;
  Observer getObserver() const;
};


#endif //RAYTRACER_MODELING_H