#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

#include <boost/program_options.hpp>


#include "../spacetime/SpacetimeKerr.h"

void progress_bar(float ratio) {
    uint8_t len = 60, end = int(len * ratio);

    std::cerr << "[";
    for (uint8_t i = 0; i < len; i++) {
        if (i < end) {
            std::cerr << "=";
        }
        else if (i == end) {
            std::cerr << ">";
        }
        else {
            std::cerr << " ";
        }
    }

    std::cerr << "] " << int(ratio * 100) << " %\r";
    std::cerr.flush();
}

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
		("stepsize", bpo::value<float>()->default_value(5e-2f), "Step size")
		("boundary", bpo::value<float>()->default_value(200.f), "Norm boundary for escape")
		("solver", bpo::value<std::string>()->default_value("rk4"), "DE solver")
		("help,h", "Produce help message")
	;
	bpo::variables_map vm;
	bpo::store(bpo::parse_command_line(argc, argv, desc), vm);
	bpo::notify(vm);

	if (vm.count("help"))
	{	
		std::cout << desc << std::endl;
		return 0;
	}
	SpacetimeKerr spacetime(
		glm::vec3(),
		vm["spin"].as<float>(),
		vm["radius"].as<float>(),
		vm["c"].as<float>(),
		vm["tolerance"].as<float>(),
		vm["solver"].as<std::string>() == "euler"
	);

	float const boundary = vm["boundary"].as<float>();
	int const nSteps = vm["steps"].as<int>();

	int nGeodesics = 0;
	std::cin >> nGeodesics;
	//std::scanf("%d\n", &nGeodesics);

	std::vector<float> px, py, pz;
	px.resize(nGeodesics);
	py.resize(nGeodesics);
	pz.resize(nGeodesics);
	for (int i = 0; i < nGeodesics; ++i)
	{
		std::cin >> px[i] >> py[i] >> pz[i];
		//std::scanf("%f %f %f\n", &px[i], &py[i], &pz[i]);
	}
	std::cerr << "Processing " << nGeodesics << " geodesics\n";

	printf("i,step,x,y,z,dx,dy,dz,h\n");
	for (int i = 0; i < nGeodesics; ++i)
	{
		// Initialise a ray
		Ray r(glm::vec3(px[i], py[i], pz[i]), glm::vec3(0, 0, -1));
		std::cerr << "Emission: " << px[i] << "," << py[i] << "," << pz[i] << '\n';
		float maxTime = vm["stepsize"].as<float>();

		for (int j = 0; j < nSteps; ++j)
		{
			r = spacetime.geodesic(r, &maxTime);
			if (std::isnan(maxTime) || !std::isfinite(maxTime))
			{
				if (std::isnan(maxTime))
					std::cerr << "nan\n";
				else
					std::cerr << "infty\n";
				break;
			}
			glm::vec3 const o = r.getOrigin();
			glm::vec3 const d = r.getDirection();
			if (boundary > 0.f && glm::l2Norm(o) > boundary)
			{
				std::cerr << "Escape\n";
				break;
			}
			printf("%d,%d,%f,%f,%f,%f,%f,%f,%f\n", i, j, o.x, o.y, o.z, d.x, d.y, d.z, maxTime);

		}
		progress_bar(i / (float) nGeodesics);
	}
	progress_bar(1.f);
}
