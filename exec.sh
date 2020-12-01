#!/bin/bash

docker run \
	-rm -v ${PWD}:/data \
	--entrypoint build/RayTracer \
	env_raytracer $@
