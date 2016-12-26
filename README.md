Indivisible
===========
[![build status](https://gitlab.com/Deathsbreed/Indivisible/badges/master/build.svg)](https://gitlab.com/Deathsbreed/Indivisible/commits/master)

Indivisible is an optimized prime number generator written in C.

Building
--------
There are multiple dependencies to install before compiling the project:
- CMake
- GMP
- OpenMP

Once the dependencies are installed you can compile by running the following from the root directory of the project:
```bash
$ cd build/
$ cmake ..
$ make
```

To build a release build run `cmake -DCMAKE_BUILD_TYPE=Release ..` instead.

Contributing
------------
If you would like to contribute to Uhn then please read the [contributing file](/CONTRIBUTING.md) before creating a merge request.

License
-------
This project is licensed with the [GNU GPLv3](/LICENSE). Since it is an educational project I find it very important that all contributions continue to remain free/libre.
