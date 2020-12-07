#ifndef SPACETIME_SPACETIME_H
#define SPACETIME_SPACETIME_H

#include <glm/glm.hpp>

#include "../render/Ray.h"

class Spacetime
{
public:
	Spacetime(int maxSteps = 1000, float initialStepSize=1e-1f)
		: maxSteps(maxSteps)
		, initialStepSize(initialStepSize)
	{}
	virtual ~Spacetime() = default;

	/**
	 * Move a ray along a null geodesic
	 *
	 * If maxTime is returned to be NaN, then it is an indication that the ray
	 * has been lost to an event horizon.
	 */
	virtual Ray
	geodesic(Ray const& ray, float* const maxTime) = 0;

	int getMaxSteps() const { return maxSteps; }
	float getInitialStepSize() const { return initialStepSize; }
private:
	int maxSteps;
	float initialStepSize;
};

#endif // !SPACETIME_SPACETIME_H
