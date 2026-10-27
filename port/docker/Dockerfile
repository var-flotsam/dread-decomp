# The port's Linux build environment: Clang, lld and the LLVM tools, CMake, Ninja, and the Python
# the port's tools need (tools/port_sweep.py, tools/port_link.py, tools/test_port_loader.py). No
# game data and no repository content goes into the image: the checkout, orig/ and build/ are
# mounted at run time (port/docker/run.sh, port/README.md "Linux in Docker").
#
#   docker build -t dread-port-linux port/docker
FROM ubuntu:24.04

ARG LLVM=18
RUN apt-get update \
 && DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
      clang-${LLVM} lld-${LLVM} llvm-${LLVM} libc++-${LLVM}-dev libc++abi-${LLVM}-dev \
      cmake ninja-build make \
      python3 python3-pyelftools python3-lz4 \
      git ca-certificates \
 && rm -rf /var/lib/apt/lists/* \
 && for t in clang clang++ ld.lld llvm-nm llvm-objdump llvm-cxxfilt llvm-readobj llvm-ar llvm-ranlib; do \
      ln -sf /usr/bin/$t-${LLVM} /usr/local/bin/$t; done

ENV CC=clang CXX=clang++
WORKDIR /src
