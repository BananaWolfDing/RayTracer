#!/bin/bash

docker run \
	--rm --interactive -v ${PWD}:/data \
	--name env_raytracer_exec \
	--entrypoint gdb \
	env_raytracer $@
