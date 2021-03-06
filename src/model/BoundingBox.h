/**
  *  Author: Zhaobo Ding (me@dingzhaobo.net)
  */
#ifndef RAYTRACER_BOUNDINGBOX_H
#define RAYTRACER_BOUNDINGBOX_H

#include <glm/glm.hpp>

#include "../render/Ray.h"

class BoundingBox
{
public:
	float x1, x2, y1, y2, z1, z2;

	BoundingBox();
	BoundingBox(float x1, float x2, float y1, float y2, float z1, float z2);

	bool hitBox(Ray ray, float tmin, float tmax) const;

	bool contains(glm::vec3 const&) const;

	BoundingBox expand(float size) const;

	/**
	 * Add a point into the bounding box
	 */
	BoundingBox operator|(glm::vec3 const&) const;
};


#endif //RAYTRACER_BOUNDINGBOX_H
