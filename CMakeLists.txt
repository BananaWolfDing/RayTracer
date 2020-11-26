cmake_minimum_required(VERSION 3.12)
project(RayTracer)

set(CMAKE_CXX_STANDARD 17)

option(BUILD_STATIC_LIBS "build static glm library" ON)

add_subdirectory(lodepng)
add_subdirectory(glm)

add_executable(RayTracer src/main.cpp src/RGB_Image.cpp src/RGB_Image.h src/render/Ray.cpp src/render/Ray.h src/model/Material.cpp src/model/Material.h src/render/Tracer.cpp src/render/Tracer.h src/model/Object.h src/render/HitRecord.cpp src/render/HitRecord.h src/model/Sphere.cpp src/model/Sphere.h src/model/Triangle.cpp src/model/Triangle.h src/model/Cube.cpp src/model/Cube.h src/model/Scene.cpp src/model/Scene.h src/model/BoundingBox.cpp src/model/BoundingBox.h)

target_link_libraries(RayTracer LodePNG glm_static)