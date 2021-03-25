# 2021-group-11

### Build dependencies

- CMake >= 3.16.3

### Supported compilers

- GNU GCC >= 9
- Clang/LLVM >= 10

### Build instructions

```shell script
git clone git@git.chalmers.se:courses/
	dit638/students/2021-group-11.git  # Fetch source

mkdir build                                # Create build location
cd build
cmake ..                                   # Configure
make                                       # Build
cd SourceFiles
./primechecker 11                          # Run program
```

