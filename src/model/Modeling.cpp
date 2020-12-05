/**
  *  Author: Zhaobo Ding (me@dingzhaobo.net)
  */
#include <iostream>
#include "yaml-cpp/yaml.h"
#include "Modeling.h"


Model::Model(const std::string &filename) {
  info = YAML::LoadFile(filename);
}

Scene *Model::getScene() const {
  if (info["objects"].IsNull()) {
    throw ModelingException("No object defined.");
  }
  if (!info["objects"].IsMap()) {
    throw ModelingException("Unrecognizable data type found for objects.");
  }

  for (const auto &obj: info["objects"]) {
    try {
      YAML::Node mat = obj.second["material"];
      float shiniess = mat["shininess"].as<float>();
      float reflection = mat["reflection"].as<float>();
      float refraction = mat["refraction"].as<float>();
      bool transparent = mat["transparent"].as<std::string>() == "true";
      glm::vec3 diffuse(
        mat["diffuse"][0].as<float>(),
        mat["diffuse"][1].as<float>(),
        mat["diffuse"][2].as<float>()
      );
      glm::vec3 specular(
        mat["specular"][0].as<float>(),
        mat["specular"][1].as<float>(),
        mat["specular"][2].as<float>()
      );
    }
    catch (std::exception e) {
      throw ModelingException("Material Error");
    }
    
    try {
      std::string type = obj.second["type"].as<std::string>();
      if (type == "sphere") {
        YAML::Node pos = obj.second["position"];
        float radius = obj.second["radius"].as<float>();
        glm::vec3 position(
          pos[0].as<float>(),
          pos[1].as<float>(),
          pos[2].as<float>()
        );
      }
      else if (type == "cube") {
        std::string type = obj.second["type"].as<std::string>();
        YAML::Node pos = obj.second["position"];
        float edge = obj.second["size"].as<float>();
        glm::vec3 position(
          pos[0].as<float>(),
          pos[1].as<float>(),
          pos[2].as<float>()
        );
      }
      else if (type == "triangle") {
        std::string type = obj.second["type"].as<std::string>();
        glm::vec3 p1(
          obj.second["p1"][0].as<float>(),
          obj.second["p1"][1].as<float>(),
          obj.second["p1"][2].as<float>()
        );
        glm::vec3 p2(
          obj.second["p2"][0].as<float>(),
          obj.second["p2"][1].as<float>(),
          obj.second["p2"][2].as<float>()
        );
        glm::vec3 p3(
          obj.second["p3"][0].as<float>(),
          obj.second["p3"][1].as<float>(),
          obj.second["p3"][2].as<float>()
        );
      }
      else {
        throw;
      }
    }
    catch (std::exception e) {
      throw ModelingException("Object Error.");
    }
  }
  Scene *s = new Scene();
  return s;
}

std::list<Light *> Model::getLights() const {
  if (info["lights"].IsNull()) {
    throw ModelingException("No light defined.");
  }
  if (!info["lights"].IsMap()) {
    throw ModelingException("Unrecognizable data type found for lights.");
  }

  for (const auto &light: info["lights"]) {
    try {
      glm::vec3 position(
        light.second["position"][0].as<float>(),
        light.second["position"][1].as<float>(),
        light.second["position"][2].as<float>()
      );
      glm::vec3 color(
        light.second["color"][0].as<float>(),
        light.second["color"][1].as<float>(),
        light.second["color"][2].as<float>()
      );
      glm::vec3 falloff(
        light.second["falloff"][0].as<float>(),
        light.second["falloff"][1].as<float>(),
        light.second["falloff"][2].as<float>()
      );
    }
    catch (std::exception e) {
      throw ModelingException("Light Error.");
    }
  }

  return std::list<Light *>();
}

Observer Model::getObserver() const {
  if (info["observer"].IsNull()) {
    throw ModelingException("No observer defined.");
  }

  try {
    glm::vec3 eye(
        info["observer"]["eye"][0].as<float>(),
        info["observer"]["eye"][1].as<float>(),
        info["observer"]["eye"][2].as<float>()
    );
    glm::vec3 ambient(
        info["observer"]["ambient"][0].as<float>(),
        info["observer"]["ambient"][1].as<float>(),
        info["observer"]["ambient"][2].as<float>()
    );
    glm::vec3 up(
        info["observer"]["up"][0].as<float>(),
        info["observer"]["up"][1].as<float>(),
        info["observer"]["up"][2].as<float>()
    );
    glm::vec3 view(
        info["observer"]["view"][0].as<float>(),
        info["observer"]["view"][1].as<float>(),
        info["observer"]["view"][2].as<float>()
    );
  }
  catch (std::exception e) {
    throw ModelingException("Observer Error.");
  }

  return Observer();
}