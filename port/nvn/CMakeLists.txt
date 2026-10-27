# The NVN shim (M2): NVN, as the game uses it, over Vulkan. Written from the game's own calls through
# its NVN function table (the nvn* names in its strings, the call sites), never from NVN's SDK
# headers or an emulator; README.md has the design. Compiled against the Khronos Vulkan headers
# (DREAD_VULKAN_INCLUDE); nothing links against a Vulkan loader yet.
add_library(dread_nvn STATIC
  src/bindings.cpp
  src/command.cpp
  src/device.cpp
  src/memory.cpp
  src/procs.cpp
  src/queue.cpp
  src/shader_manifest.cpp
  src/state.cpp
  src/window.cpp)
target_include_directories(dread_nvn PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(dread_nvn SYSTEM PUBLIC "${DREAD_VULKAN_INCLUDE}")
target_link_libraries(dread_nvn PUBLIC dread_stdlib)

# nvn_manifest_check: runs the manifest reader and the binding layer over the real build output
# (no GPU, no Vulkan loader); check/manifest_check.cpp lists the checks. Needs the `shaders`
# target's output and the dump:
#   nvn_manifest_check <DREAD_SHADER_DIR> orig/<version>/romfs/system/shd
if(DREAD_LINK)
  add_executable(nvn_manifest_check check/manifest_check.cpp)
  target_link_libraries(nvn_manifest_check PRIVATE dread_nvn)
endif()

# nvn_vulkan_check: the binding layer against a real Vulkan implementation, with the validation
# layers (check/vulkan_check.cpp). Built when a Vulkan loader is found, as in the port's Vulkan image
# (port/docker/vulkan/Dockerfile: the Khronos loader, lavapipe, the validation layers):
#   nvn_vulkan_check <DREAD_SHADER_DIR>
# Only the loader library is taken from the system; the headers stay the pinned ones.
if(DREAD_LINK)
  find_library(DREAD_VULKAN_LIBRARY NAMES vulkan vulkan-1)
  if(DREAD_VULKAN_LIBRARY)
    add_executable(nvn_vulkan_check check/vulkan_check.cpp)
    target_link_libraries(nvn_vulkan_check PRIVATE dread_nvn "${DREAD_VULKAN_LIBRARY}")
    # The game's graphics start-up and frame loop replayed against the shim (check/startup_check.cpp).
    add_executable(nvn_startup_check check/startup_check.cpp)
    target_link_libraries(nvn_startup_check PRIVATE dread_nvn "${DREAD_VULKAN_LIBRARY}")
    message(STATUS "dread: Vulkan loader ${DREAD_VULKAN_LIBRARY}; nvn_vulkan_check and nvn_startup_check are built")
  else()
    message(STATUS "dread: no Vulkan loader; nvn_vulkan_check is not built")
  endif()
endif()
