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
