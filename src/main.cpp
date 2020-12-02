#include <iostream>
#include <glm/glm.hpp>

#include <boost/program_options.hpp>


#include "RGB_Image.h"
#include "model/Scene.h"
#include "render/Tracer.h"

#include "spacetime/SpacetimeFlat.h"
#include "spacetime/SpacetimeKerr.h"

int main(int argc, char* argv[])
{
	namespace bpo = boost::program_options;
	bpo::options_description desc("Raytracer");
	desc.add_options()
		("threads,t", bpo::value<int>()->default_value(1), "Number of threads")
		("output,o", bpo::value<std::string>()->default_value("untitled.png"), "Output file")
		;
	bpo::variables_map vm;
	bpo::store(bpo::parse_command_line(argc, argv, desc), vm);

	std::cout << "Rendering with " << vm["threads"].as<int>() << " threads"
	          << std::endl;
	
	RGB_Image img(800, 800);
	Scene scene = Scene();
	
	float fall[3] = {1, 0, 0};
	Light light(glm::vec3(0.8, 0.8, 0.8), glm::vec3(100, 100, 100), fall);
	std::list<Light *> lights;
	lights.emplace_back(&light);
	
	//SpacetimeFlat spacetime;
	SpacetimeKerr spacetime(.9f, .8f, 10.f);
	
	Tracer tracer(
	        &scene,
	        &img,
	        glm::vec3(0, 100, 100),
	        glm::vec3(0, 0, 0),
	        glm::vec3(0, 1, 0),
	        glm::vec3(0, -.707, -.707),
	        50.0f,
	        lights,
	        &spacetime,
	        false,
	        vm["threads"].as<int>()
	        );
	
	tracer.render();
	img.export_image(vm["output"].as<std::string>());
	return 0;
}
