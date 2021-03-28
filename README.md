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

### New features

New features will be added by creating a separate branch based of the latest master. When the feature is complete, i.e compiles, builds and runs without errors
the person responsible for this feature will create a merge request to master. Another team member will review the code and if accepted it shall be merged to master
