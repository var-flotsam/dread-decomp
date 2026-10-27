# The port's own nn:: runtime: declarations under include/nn/, written from the game's uses
# (src/) and the imported symbol names (data/functions/), with stub implementations under src/.
# Nothing here comes from lib/nnsdk. Each stub names the boot-path function that needs it in a
# TODO; a namespace with no caller yet is a header with a comment on what the boot path will need.
# include/dread_port.h is the runtime's own surface: the trap behind the functions the port has no
# source for (src/missing.cpp), which the thunks tools/port_link.py generates jump to.
# include/dread_port_loader.h is the data loader: src/loader.cpp (freestanding, tested on its own by
# tools/test_port_loader.py) and src/image.cpp (the host layer, which maps the dump before the
# game's static initialisers when the game is linked in).
add_library(dread_runtime STATIC
  src/account.cpp
  src/diag.cpp
  src/fault.cpp
  src/fs.cpp
  src/hid.cpp
  src/image.cpp
  src/loader.cpp
  src/mem.cpp
  src/missing.cpp
  src/movie.cpp
  src/nv.cpp
  src/os.cpp
  src/oe.cpp
  src/prepo.cpp
  src/time.cpp
  src/util.cpp
  src/vi.cpp)
target_include_directories(dread_runtime PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
  "${DREAD_ROOT}/include")
target_link_libraries(dread_runtime PUBLIC dread_stdlib)
