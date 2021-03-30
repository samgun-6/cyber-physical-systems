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

New features will be added by creating a new issue and then a branch based from this issue. When the feature is complete, i.e compiles, builds and runs without errors
the person responsible for this feature will create a merge request to master. Another team member will review the code and if accepted it shall be merged to master

### Fixing unexpected behavior

To minimize the risks of having unexpected behavior we will contuct a code review on merge request, where the mergee will assign a group member to look through the code. If however an unexpected behavior is detected on the master branch it will be reverted until the lastest stable commit. The person who wrote the code that introduced a bug will be assigned to find the bug and fix it, this will be done by creating a branch "bugfix" that is based from the specific commit that introduced the bug. Depending on the severity of the bug, i.e if it is just a minor bug creating a new branch based of master and fixing the bug is enough.  


### Structure and content of commit messages

The header of the commit message shall be a short descriptive message of what the specific commit does. A body to the commit message is not required on all commit messages, only those who need further explanations i.e if you can not explaint what the specific commit does by just using the header

