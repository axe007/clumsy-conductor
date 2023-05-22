# Start from the base image
FROM --platform=$BUILDPLATFORM ubuntu:20.04 AS builder

# Specify the maintainer
LABEL maintainer="maintainer@example.com"

# Set the environment variable
ENV DEBIAN_FRONTEND noninteractive

# Install necessary build tools and libraries
RUN rm -rf /var/lib/apt/lists/* && \
    apt-get clean && \
    apt-get update --fix-missing && \
    apt-get install -y protobuf-compiler build-essential python3-sysv-ipc python3-numpy python3-opencv protobuf-compiler software-properties-common && \
    add-apt-repository -y ppa:chrberger/libcluon && \
    apt-get update && \
    apt-get upgrade -y && \
    apt-get dist-upgrade -y && \
    apt-get install -y --no-install-recommends libcluon && \
    apt-get clean

# Copy the source code to the working directory
COPY ./src /opt/sources

# Set the working directory
WORKDIR /opt/sources

# Build the project
RUN make

# Create the final image
FROM ubuntu:20.04

# Set the environment variable
ENV DEBIAN_FRONTEND noninteractive

# Install Python3 and other dependencies
RUN rm -rf /var/lib/apt/lists/* && \
    apt-get clean && \
    apt-get update --fix-missing && \
    apt-get install -y python3 python3-sysv-ipc python3-numpy python3-opencv protobuf-compiler && \
    apt-get clean

# Copy the compiled program from the builder
COPY --from=builder /opt/sources /opt/sources

# Set the working directory
WORKDIR /opt/sources

# Define the command to run when the container starts
CMD ["python3", "/opt/sources/main.py"]