##################################################
# Section 1: Build the application
FROM ubuntu:20.04 as builder
ARG DEBIAN_FRONTEND=noninteractive
MAINTAINER Christian Berger christian.berger@gu.se

RUN apt-get update -y && \
    apt-get upgrade -y && \
    apt-get dist-upgrade -y

RUN apt-get install -y --no-install-recommends \
        cmake \
        build-essential

ADD . /opt/sources
WORKDIR /opt/sources

RUN cd /opt/sources && \
    mkdir build && \
    cd build && \
    cmake -D CMAKE_BUILD_TYPE=Release .. && \
    make && cd tests/ && ./testsPrimeChecker && cd .. && cp SourceFiles/primecheckerSource /tmp

##################################################
# Section 2: Bundle the application.
FROM ubuntu:20.04
MAINTAINER Christian Berger christian.berger@gu.se

RUN apt-get update -y && \
    apt-get upgrade -y && \
    apt-get dist-upgrade -y

WORKDIR /opt
COPY --from=builder /tmp/primecheckerSource .
ENTRYPOINT ["/opt/primecheckerSource"]

