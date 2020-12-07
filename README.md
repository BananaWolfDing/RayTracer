# DZB's General Relativity Renderer

## Build

To build (this actually runs `docker-compose`)
```sh
../build.sh
```

## Run

To see the manual:
```sh
./exec.sh --help
```

Example:
```sh
./exec.sh --input examples/simple.yml
```

## Diagnostics

To debug:
```sh
./debug.sh gdb -ex run --args build/RayTracer ARGS
```

The Geodesic utility can trace a list of geodesics in the Kerr spacetime and
output the path in csv format.
```sh
./test.sh Geodesic --help
```
