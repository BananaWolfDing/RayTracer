
set(SRC_FILES
	RGB_Image.cpp
	model/BoundingBox.cpp
	model/Cube.cpp
	model/Light.cpp
	model/Material.cpp
	model/Modeling.cpp
	model/Object.cpp
	model/Scene.cpp
	model/Sphere.cpp
	model/Triangle.cpp
	model/bump/BumpMapping.cpp
	render/HitRecord.cpp
	render/Ray.cpp
	render/Tracer.cpp
	spacetime/SpacetimeFlat.cpp
	spacetime/SpacetimeKerr.cpp
)

add_executable(RayTracer main.cpp ${SRC_FILES})
target_link_libraries(RayTracer
	${CMAKE_THREAD_LIBS_INIT}
	${Boost_LIBRARIES}
	yaml-cpp
	LodePNG
	)

add_executable(Geodesic
	test/Geodesic.cpp
	render/Ray.cpp
	spacetime/SpacetimeFlat.cpp
	spacetime/SpacetimeKerr.cpp
	)
target_link_libraries(Geodesic
	${CMAKE_THREAD_LIBS_INIT}
	${Boost_LIBRARIES}
	)
target_compile_definitions(Geodesic PUBLIC DEBUG_GEODESIC)
