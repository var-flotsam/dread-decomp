# The port's own nn:: runtime: declarations under include/nn/, written from the game's uses
# (src/) and the imported symbol names (data/functions/), with stub implementations under src/.
# Nothing here comes from lib/nnsdk. Each stub names the boot-path function that needs it in a
# TODO; a namespace with no caller yet is a header with a comment on what the boot path will need.
add_library(dread_runtime STATIC
  src/os.cpp
  src/oe.cpp
  src/vi.cpp)
target_include_directories(dread_runtime PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
  "${DREAD_ROOT}/include")
target_link_libraries(dread_runtime PUBLIC dread_stdlib)
