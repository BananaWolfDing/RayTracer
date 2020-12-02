cmake_minimum_required(VERSION 3.12)
project(RayTracer)

set(PROJECT_SOURCE_DIR     ${CMAKE_SOURCE_DIR}/src)
set(CMAKE_BINARY_DIR       ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(CMAKE_CXX_STANDARD 17)
find_package(Threads REQUIRED)
find_package(Boost COMPONENTS
	program_options
	thread
	timer
	REQUIRED)

if (${Boost_FOUND})
	include_directories(${Boost_INCLUDE_DIRS})
	link_directories(${Boost_LIBRARY_DIR})
endif()

option(BUILD_STATIC_LIBS "build static glm library" ON)

add_subdirectory(lodepng)
add_subdirectory(src)
