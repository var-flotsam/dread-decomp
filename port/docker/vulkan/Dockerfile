# The port's Linux build image (port/docker/Dockerfile) plus Vulkan with no GPU: the Khronos loader
# and headers, Mesa's software driver lavapipe and the Khronos validation layers, all Ubuntu
# packages. For the NVN shim's checks against a real Vulkan implementation (port/nvn/README.md);
# the M1 build does not need it. No game data goes into the image.
#
#   docker build -t dread-port-linux port/docker
#   docker build -t dread-port-linux-vulkan port/docker/vulkan
#   DREAD_PORT_IMAGE=dread-port-linux-vulkan port/docker/run.sh vulkaninfo --summary
ARG BASE=dread-port-linux
FROM ${BASE}

RUN apt-get update \
 && DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
      libvulkan-dev mesa-vulkan-drivers vulkan-validationlayers vulkan-tools \
 && rm -rf /var/lib/apt/lists/*

# Only lavapipe: a container has no GPU, and a fixed driver keeps the checks reproducible.
ENV VK_DRIVER_FILES=/usr/share/vulkan/icd.d/lvp_icd.json
