
set(SRC_FILES
	main.cpp
	RGB_Image.cpp
	render/Ray.cpp
	model/Material.cpp
	render/Tracer.cpp
	render/HitRecord.cpp
	model/Sphere.cpp
	model/Triangle.cpp
	model/Cube.cpp
	model/Scene.cpp
	model/BoundingBox.cpp
	model/Light.cpp
	model/Object.cpp
	model/Modeling.cpp
	model/bump/BumpMapping.cpp
)

add_executable(RayTracer ${SRC_FILES})
target_link_libraries(RayTracer
	${CMAKE_THREAD_LIBS_INIT}
	yaml-cpp
	LodePNG
	)


