
set(SRC_FILES
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
	spacetime/SpacetimeFlat.cpp
	spacetime/SpacetimeKerr.cpp
)

add_executable(RayTracer main.cpp ${SRC_FILES})
target_link_libraries(RayTracer
	${CMAKE_THREAD_LIBS_INIT}
	${Boost_LIBRARIES}
	LodePNG
	)

add_executable(Geodesic test/Geodesic.cpp ${SRC_FILES})
target_link_libraries(Geodesic
	${CMAKE_THREAD_LIBS_INIT}
	${Boost_LIBRARIES}
	LodePNG
	)
