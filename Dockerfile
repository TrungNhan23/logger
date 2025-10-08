# Base it on a clean Ubuntu image or your own corporate base image
FROM ubuntu:22.04


# --- Export them as environment variables for RUN ---
ENV HTTP_PROXY=${HTTP_PROXY}
ENV HTTPS_PROXY=${HTTPS_PROXY}
ENV NO_PROXY=${NO_PROXY}
ENV http_proxy=${HTTP_PROXY}
ENV https_proxy=${HTTPS_PROXY}
ENV no_proxy=${NO_PROXY}
RUN echo "Using proxy: $HTTP_PROXY"

# Install build dependencies for Logger project
RUN apt-get update && \
    apt-get install -y sudo --no-install-recommends \
        build-essential \
        cmake \
        clang \
        clang-tidy \
        python3 \
        git \
        libgtest-dev \
        curl && \
    rm -rf /var/lib/apt/lists/*

# Build and install GoogleTest (Ubuntu ships sources only)
RUN cd /usr/src/googletest && cmake . && make && cp lib/*.a /usr/local/lib

# Optional: add user if you don’t want root
RUN useradd -ms /bin/bash runner
RUN echo "runner ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers

USER runner
WORKDIR /home/runner
