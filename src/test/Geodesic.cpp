#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

#include <boost/program_options.hpp>


#include "../spacetime/SpacetimeKerr.h"

int main(int argc, char* argv[])
{
	namespace bpo = boost::program_options;
	bpo::options_description desc("Geodesic test utility");
	desc.add_options()
		("metric", bpo::value<std::string>()->default_value("kerr"), "Metric used (only kerr is supported)")
		("steps,n", bpo::value<int>()->default_value(1000), "Number of steps")
		("spin", bpo::value<float>()->default_value(.05f), "Kerr spin")
		("radius", bpo::value<float>()->default_value(10.f), "Schwarzschild Radius")
		("c", bpo::value<float>()->default_value(10.f), "Speed of light")
		("tolerance", bpo::value<float>()->default_value(5e-2f), "Tolerance")
		("px", bpo::value<float>()->multitoken(), "Emission x (list)")
		("py", bpo::value<float>()->multitoken(), "Emission y (list)")
		("pz", bpo::value<float>()->multitoken(), "Emission z (list)")
		("help,h", "Produce help message")
	;
	bpo::variables_map vm;
	bpo::store(bpo::parse_command_line(argc, argv, desc), vm);

	if (vm.count("help"))
	{	
		std::cout << desc << std::endl;
		return 0;
	}
	SpacetimeKerr spacetime(
		vm["spin"].as<float>(),
		vm["radius"].as<float>(),
		vm["c"].as<float>(),
		vm["tolerance"].as<float>()
	);

	int const nSteps = vm["steps"].as<int>();

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
