#ifndef SPACETIME_SPACETIMEKERR_H
#define SPACETIME_SPACETIMEKERR_H

#include "Spacetime.h"

class SpacetimeKerr final: public Spacetime
{
public:
	SpacetimeKerr(float kerr, float rs)
		: kerr(kerr)
		, rs(rs)
		{}

	virtual Ray
	geodesic(Ray const& ray, float* const maxTime) override;

private:
	/**
	 * Kerr parameter dictating the ratio of angular momentum to rotating mass
	 * times the speed of light.
	 *
	 * a = J/(M*c)
	 */
	float kerr;
	/**
	 * Schwarzschild radius of the mass. Given by
	 *
	 * r_s = 2*G*M/c^2
	 */
	float rs;
};

#endif // !SPACETIME_SPACETIMEKERR_H
