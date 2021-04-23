# 2021-group-11

[![pipeline status](https://git.chalmers.se/courses/dit638/students/2021-group-11/badges/master/pipeline.svg)](https://git.chalmers.se/courses/dit638/students/2021-group-11/-/commits/master)

### Build dependencies

- CMake >= 3.16.3

### Supported compilers

- GNU GCC >= 9
- Clang/LLVM >= 10

### Build instructions

```shell script
git clone git@git.chalmers.se:courses/
	dit638/students/2021-group-11.git  # Fetch source

1. Below command has to be executed within a folder that contain rec files 
docker run --rm --init --net=host --name=opendlv-vehicle-view -v $PWD:/opt/vehicle-view/recordings -v /var/run/docker.sock:/var/run/docker.sock -p 8081:8081 chalmersrevere/opendlv-vehicle-view-multi:v0.0.60

2.a - Run below command if h264 decoder is not already built as an image
docker build https://github.com/chalmers-revere/opendlv-video-h264-decoder.git#v0.0.4 -f Dockerfile.amd64 -t h264decoder:v0.0.4

2.b
docker run --rm -ti --net=host --ipc=host -e DISPLAY=$DISPLAY -v /tmp:/tmp h264decoder:v0.0.4 --cid=253 --name=img

3. Must be exectued within the same folder as the Dockerfile of the project
docker build -f Dockerfile -t somename .

4.
docker run --rm -ti --net=host --ipc=host -e DISPLAY=$DISPLAY -v /tmp:/tmp somename:latest --cid=253 --name=img --width=640 --height=480 --verbose

```

### New features

New features will be added by creating a new issue and then a branch based from this issue. When the feature is complete, i.e compiles, builds and runs without errors
the person responsible for this feature will create a merge request to master. Another team member will review the code and if accepted it shall be merged to master

### Fixing unexpected behavior

To minimize the risks of having unexpected behavior we will conduct a code review on merge requests, where the mergee will assign a group member to look through the code and approve the merge. If an unexpected behaviour is detected on the master branch after the merge we will adhere to the following steps:

A hotfix is when fixing is conducted right away after finding it.
A bugfix is when the fixing can be time consuming including major fixes. they are usually conducted in the planned work sessions

1. Create a issue that explains the bug, then depending on the severity of the bug create a branch from this issue straight away and fix it or postpone it when there is time available. If the bug is minor, i.e a "hotfix" create a new branch straight away and fix the bug.
2. Checkout to the branch and fix the bug
3. Create a merge request to the master branch
4. Review and accept (or decline) the merge request

If other feature branches are active and are based on the master version with the bug we will pull the "fixed" master branch into that feature branch to be able to work on the latest version.

### Structure and content of commit messages

The subject line of the commit message shall be a short descriptive message of what the specific commit does. A body to the commit message is not required on all commit messages, only those who need further explanations i.e if you cannot explain what the specific commit does by just using the subject line. The subject line should be short and descriptive and we recommend 50 characters or less but a maximum of 72 characters so if longer it should go into the body. The subject line should be able to complete the sentence "If applied, this commit will": [specify your subject line]. The body should answer the "what" and "why".

