cmake_minimum_required(VERSION 3.12)
project(RayTracer)

set(PROJECT_SOURCE_DIR     ${CMAKE_SOURCE_DIR}/src)
set(CMAKE_BINARY_DIR       ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(CMAKE_CXX_STANDARD 17)
find_package(Threads REQUIRED)

option(BUILD_STATIC_LIBS "build static glm library" ON)

add_subdirectory(lodepng)
add_subdirectory(src)
