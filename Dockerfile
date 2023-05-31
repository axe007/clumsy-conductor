#
# Scribbled on by Group 16, Class of 2023
#
# Use Docker buildx for multi-arch build
# docker buildx create --name multiarch --driver docker-container --use
# docker buildx inspect --bootstrap
# docker buildx build --platform linux/amd64,linux/arm/v7 -t clumsy-conductor .
# And fire away

# First stage for building the software:
FROM ubuntu:18.04 as builder

ENV DEBIAN_FRONTEND noninteractive

# Upgrade the Ubuntu 18.04 LTS base image
RUN apt-get update -y && \
    apt-get upgrade -y && \
    apt-get dist-upgrade -y && \
    apt-get install -y --no-install-recommends \
        ca-certificates \
        cmake \
        build-essential \
        libopencv-dev

# Include this source tree and compile the sources
ADD . /opt/sources
WORKDIR /opt/sources
RUN mkdir build && \
    cd build && \
    cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=/tmp .. && \
    make && make install

# Second stage for packaging the software into a software bundle:
FROM ubuntu:18.04

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update -y && \
    apt-get upgrade -y && \
    apt-get dist-upgrade -y

RUN apt-get install -y --no-install-recommends \
        libopencv-core3.2 \
        libopencv-highgui3.2 \
        libopencv-imgproc3.2 

WORKDIR /usr/bin
COPY --from=builder /tmp/bin/clumsy-conductor .
# This is the entrypoint when starting the Docker container; hence, this Docker image is automatically starting our software on its creation
ENTRYPOINT ["/usr/bin/clumsy-conductor"]
