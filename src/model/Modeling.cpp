/**
  *  Author: Zhaobo Ding (me@dingzhaobo.net)
  */
#include <iostream>
#include <cmath>
#include "Modeling.h"
#include "Cube.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Material.h"

#include "../spacetime/SpacetimeFlat.h"
#include "../spacetime/SpacetimeKerr.h"


Model::Model(const std::string &filename) {
  info = YAML::LoadFile(filename);
}

Scene *Model::getScene(std::vector<Object *> &itemsList) const {
  if (info["objects"].IsNull()) {
    throw ModelingException("No object defined.");
  }
  if (!info["objects"].IsMap()) {
    throw ModelingException("Unrecognizable data type found for objects.");
  }

  std::vector<Object *> objectList;
  float x1, x2, y1, y2, z1, z2;

  for (const auto &obj: info["objects"]) {
    Material material;
    Object *item;
    try {
      YAML::Node mat = obj.second["material"];
      float shininess = mat["shininess"].as<float>();
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

      material = Material(
        diffuse, specular, shininess,
        reflection, refraction, transparent);
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

        item = new Sphere(position, radius);
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

        item = new Cube(position, edge);
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

        item = new Triangle(p1, p2, p3);
      }
      else {
        throw ModelingException("Unrecognized type for object.");
      }

      item->setMaterial(material);
      objectList.push_back(item);
    }
    catch (std::exception e) {
      throw ModelingException("Object Error.");
    }
  }

  try {
    x1 = info["space"]["x"][0].as<float>();
    x2 = info["space"]["x"][1].as<float>();
    y1 = info["space"]["y"][0].as<float>();
    y2 = info["space"]["y"][1].as<float>();
    z1 = info["space"]["z"][0].as<float>();
    z2 = info["space"]["z"][1].as<float>();
  }
  catch (std::exception e) {
    throw ModelingException("Space Error.");
  }

  Scene *s = new Scene(
    objectList, x1, x2, y1, y2, z1, z2, int(8 + 1.3 * log(objectList.size()))
  );
  itemsList = objectList;
  return s;
}

std::list<Light *> Model::getLights() const {
  if (info["lights"].IsNull()) {
    throw ModelingException("No light defined.");
  }
  if (!info["lights"].IsMap()) {
    throw ModelingException("Unrecognizable data type found for lights.");
  }

  std::list<Light *> lights;
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
      float falloff[] = {
        light.second["falloff"][0].as<float>(),
        light.second["falloff"][1].as<float>(),
        light.second["falloff"][2].as<float>()
      };

      Light *instance = new Light(color, position, falloff);
      lights.push_back(instance);
    }
    catch (std::exception e) {
      throw ModelingException("Light Error.");
    }
  }

  return lights;
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
    float fovy = info["observer"]["fovy"].as<float>();

    return Observer(eye, ambient, up, view, fovy);
  }
  catch (std::exception e) {
    throw ModelingException("Observer Error.");
  }
}

Spacetime* Model::getSpacetime() const
{
  if (!info["spacetime"] || info["spacetime"].IsNull()) {
  	return new SpacetimeFlat;
  }

  auto obj = info["spacetime"];

	if (obj["type"].as<std::string>() == "flat")
		return new SpacetimeFlat;
	else if (obj["type"].as<std::string>() == "kerr")
	{
		float const spin = obj["spin"] ? obj["spin"].as<float>() : 0.5f;
		float const radius = obj["radius"] ? obj["radius"].as<float>() : 0.1f;
		float const c = obj["c"] ? obj["c"].as<float>() : 10.f;
		float const tolerance = obj["tolerance"] ? obj["tolerance"].as<float>() : 5e-2;
		glm::vec3 const position = obj["position"]
			? glm::vec3(
					obj["position"][0].as<float>(),
					obj["position"][1].as<float>(),
					obj["position"][2].as<float>()
				)
			: glm::vec3();
		bool const eulerSolver = obj["solver"] && obj["solver"].as<std::string>() == "euler";
		int const maxSteps = obj["maxsteps"] ? obj["maxsteps"].as<int>() : 1000;
		float const stepsize = obj["stepsize"] ? obj["stepsize"].as<float>() : 1e-1f;

		float const mass = radius * c * c / (2 * /*G=*/6.674e-11f);
		std::cout << "Black hole mass: " << mass << " kg\n";

		if (radius == 0.f)
			return new SpacetimeFlat;
		else
			return new SpacetimeKerr(position,spin, radius, c, tolerance, eulerSolver, maxSteps, stepsize);
	}

  throw ModelingException("Unknown spacetime type");
}
