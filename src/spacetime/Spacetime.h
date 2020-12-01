#ifndef SPACETIME_SPACETIME_H
#define SPACETIME_SPACETIME_H

#include <glm/glm.hpp>

#include "../render/Ray.h"

class Spacetime
{
public:
	virtual ~Spacetime() = default;

	/**
	 * Move a ray along a null geodesic
	 *
	 * If maxTime is returned to be NaN, then it is an indication that the ray
	 * has been lost to an event horizon.
	 */
	virtual Ray
	geodesic(Ray const& ray, float* const maxTime) = 0;
};

#endif // !SPACETIME_SPACETIME_H
