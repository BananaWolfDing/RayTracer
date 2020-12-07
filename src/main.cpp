#include <fstream>
#include <iostream>
#include <glm/glm.hpp>

#include <boost/program_options.hpp>


#include "RGB_Image.h"
#include "model/Scene.h"
#include "render/Tracer.h"
#include "model/Modeling.h"

int main(int argc, char* argv[])
{
	// Parse program arguments
	namespace bpo = boost::program_options;
	bpo::options_description desc("Raytracer");
	desc.add_options()
		("input,i", bpo::value<std::string>(), "Input file")
		("output,o", bpo::value<std::string>()->default_value("untitled.png"), "Output file")
		("threads,t", bpo::value<int>()->default_value(1), "Number of threads")
		("width", bpo::value<int>()->default_value(800), "Image width")
		("height", bpo::value<int>()->default_value(800), "Image height")
		("antialias", bpo::bool_switch()->default_value(false), "Use antialiasing")
		("help,h", "Produce help message")
	;
	bpo::variables_map vm;
	bpo::store(bpo::parse_command_line(argc, argv, desc), vm);

	if (vm.count("help"))
	{	
		std::cout << desc << std::endl;
		return 0;
	}
	if (!vm.count("input"))
	{
		std::cerr << "An input path must be provided\n";
		return -1;
	}
	if (vm["width"].as<int>() <= 0 || vm["height"].as<int>() <= 0)
	{
		std::cerr << "Invalid dimensions\n";
		return -1;
	}

	std::cout << "Rendering with " << vm["threads"].as<int>() << " threads"
	          << std::endl;

	Model model(vm["input"].as<std::string>());

	std::vector<Object*> items;
	Scene* s = model.getScene(items);

	std::list<Light*> lights = model.getLights();
	Observer ob = model.getObserver();

	RGB_Image img(vm["width"].as<int>(), vm["height"].as<int>());
	Spacetime* const spacetime = model.getSpacetime();

	std::cout << "Model loaded!" << std::endl;
	Tracer tracer(
	  s,
	  &img,
	  ob.eye,
	  ob.ambient,
	  ob.up,
	  ob.view,
	  ob.fovy,
	  lights,
	  spacetime,
	  vm["antialias"].as<bool>(),
	  vm["threads"].as<int>()
	);

	tracer.render();
	img.export_image(vm["output"].as<std::string>());

	for (Object* obj : items)
	{
		delete obj;
	}
	for (Light* light : lights)
	{
		delete light;
	}
	delete s;
	delete spacetime;

	return 0;
}
