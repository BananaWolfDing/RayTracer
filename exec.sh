#!/bin/bash

docker run \
	--rm -v ${PWD}:/data \
	--name env_raytracer_exec \
	--entrypoint build/RayTracer \
	env_raytracer $@
