#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

#include "../spacetime/SpacetimeKerr.h"

int main(int argc, char* argv[])
{
	SpacetimeKerr spacetime(.05f, 10.f, 10.f, 5e-2);

	int const nSteps = 1000;

	printf("i,step,x,y,z,dx,dy,dz,h\n");
	for (int i = 0; i < 10; ++i)
	{
		std::cerr << "Line " << i << "\n";
		// Initialise a ray
		Ray r(glm::vec3(10.1f + (float) i / 10.f, 0.f, 100.f), glm::vec3(0, 0, -1));
		float maxTime = 1.f;

		for (int j = 0; j < nSteps; ++j)
		{
			r = spacetime.geodesic(r, &maxTime);
			glm::vec3 const o = r.getOrigin();
			glm::vec3 const d = r.getDirection();
			if (glm::l2Norm(o) > 200)
				break;
			printf("%d,%d,%f,%f,%f,%f,%f,%f,%f\n", i, j, o.x, o.y, o.z, d.x, d.y, d.z, maxTime);
			fprintf(stderr, "%d,h=%f\n", j, maxTime);
		}
	}
}
