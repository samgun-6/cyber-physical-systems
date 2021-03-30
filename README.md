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

To minimize the risks of having unexpected behavior we will conduct a code review on merge requests, where the mergee will assign a group member to look through the code and approve the merge. If an unexpected behaviour is detected on the master branch after the merge we will adhere to the following steps:

1. Create a branch based on the current master version called hotfix or bugfix followed by a "-" and either created issue or feature for example hotfix-iss01 or bugfix-feature1
2. Checkout to the branch and fix the bug
3. Create a merge request to the master branch
4. Review and accept (or decline) the merge request

If other feature branches are active and are based on the master version with the bug we will pull the "fixed" master branch into that feature branch to be able to work on the latest version.

### Structure and content of commit messages

The subject line of the commit message shall be a short descriptive message of what the specific commit does. A body to the commit message is not required on all commit messages, only those who need further explanations i.e if you cannot explain what the specific commit does by just using the subject line. The subject line should be short and descriptive and we recommend 50 characters or less but a maximum of 72 characters so if longer it should go into the body. The subject line should be able to complete the sentence "If applied, this commit will": [specify your subject line]. The body should answer the "what" and "why".

