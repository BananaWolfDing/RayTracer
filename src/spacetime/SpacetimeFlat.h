#ifndef SPACETIME_SPACETIMEFLAT_H
#define SPACETIME_SPACETIMEFLAT_H

#include "Spacetime.h"

class SpacetimeFlat final: public Spacetime
{
public:
	virtual Ray
	geodesic(Ray const& ray, float* const maxTime) override;
};

#endif // !SPACETIME_SPACETIMEFLAT_H
