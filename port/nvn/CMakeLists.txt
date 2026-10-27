# The NVN shim (M2): NVN, as the game uses it, over Vulkan. Written from the game's own calls through
# its NVN function table (the nvn* names in its strings, the call sites), never from NVN's SDK
# headers or an emulator; README.md has the design. Compiled against the Khronos Vulkan headers
# (DREAD_VULKAN_INCLUDE); nothing links against a Vulkan loader yet.
add_library(dread_nvn STATIC
  src/bindings.cpp
  src/shader_manifest.cpp)
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
