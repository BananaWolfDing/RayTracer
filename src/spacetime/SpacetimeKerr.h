#ifndef SPACETIME_SPACETIMEKERR_H
#define SPACETIME_SPACETIMEKERR_H

#include <cmath>

#include "Spacetime.h"

class SpacetimeKerr final: public Spacetime
{
public:
	SpacetimeKerr(glm::vec3 position, float spin, float rs, float c, float epsilon=5e-2);

	virtual Ray
	geodesic(Ray const& ray, float* const maxTime) override;

	glm::vec4 dx2ds(glm::vec4 const& x, glm::vec4 const& dxds) const;

	bool is_extremal() const { return std::isnan(horizon); }

	float kerr_a() const { return spin * rs / (2 * c * c); }

private:
	glm::vec3 position;
	/**
	 * Spin parameter, given by spin = c*J/GM^2
	 */
	float spin;
	/**
	 * Schwarzschild radius of the mass. Given by
	 *
	 * r_s = 2*G*M/c^2
	 */
	float rs;
	/**
	 * Speed of light
	 */
	float c;

	/**
	 * Radius of event horizon in Boyer-Lindquist Coordinates. If nan then it
	 * represents a extremal black hole.
	 */
	float horizon;

	float epsilon;
};

#endif // !SPACETIME_SPACETIMEKERR_H
