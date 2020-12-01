#!/bin/bash

docker run \
	-v ${PWD}:/data \
	--entrypoint build/RayTracer \
	env_raytracer $@
