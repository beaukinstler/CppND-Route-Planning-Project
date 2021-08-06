# Route Planning Project

This repo contains the starter code for the Route Planning project.

<img src="map.png" width="600" height="450" />

# Introduction

A project to try out some c++, mostly standard library. The primary purpose is to implment A*Search over a map, and plot the corse.

## Installation

Git close this repo. See the README [here](https://github.com/udacity/CppND-Route-Planning-Project/blob/master/README.md) directions for installing this, as well its dependencies. 

## Compiling and running

### Compiling
To compile the project, first, create a `build` directory and change to that directory:
```
mkdir build && cd build
```
From within the `build` directory, then run `cmake` and `make` as follows:
```
cmake -DCMAKE_BUILD_TYPE=Debug ../
make
```
### Running
The executable will be placed in the `build` directory. From within `build`, you can run the project as follows:
```
./OSM_A_star_search
```
Or to specify a map file:
```
./OSM_A_star_search -f ../<your_osm_file.osm>
```

## Testing

The testing executable is also placed in the `build` directory. From within `build`, you can run the unit tests as follows:
```
./test
```

