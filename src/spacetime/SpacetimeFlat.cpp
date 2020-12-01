#include "SpacetimeFlat.h"

#include <cassert>

Ray SpacetimeFlat::geodesic(Ray const& ray, float* const maxTime)
{
	assert(std::isfinite(*maxTime));

	Ray ray2(ray.getOrigin() + ray.getDirection() * *maxTime, ray.getDirection());
	*maxTime = std::numeric_limits<float>::max();
	return ray2;
}
