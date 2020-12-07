#!/bin/bash

docker run \
	--rm -v ${PWD}:/data \
	--name env_raytracer_exec \
	--interactive \
	--entrypoint build/$1 \
	env_raytracer ${@:2}
