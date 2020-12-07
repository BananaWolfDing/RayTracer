#include "SpacetimeFlat.h"

#include <cassert>

SpacetimeFlat::SpacetimeFlat()
	: Spacetime(2, std::numeric_limits<float>::infinity())
{
}
Ray SpacetimeFlat::geodesic(Ray const& ray, float* const maxTime)
{
	assert(std::isfinite(*maxTime));

	Ray ray2(ray.getOrigin() + ray.getDirection() * *maxTime, ray.getDirection());
	*maxTime = std::numeric_limits<float>::infinity();
	return ray2;
}
