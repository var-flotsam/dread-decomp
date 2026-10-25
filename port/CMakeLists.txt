# The PC port build (docs/PORT_PLAN.md, Route B). Targets:
#   dread_runtime  the port's own nn:: declarations and stubs (runtime/)
#   dread_game     every source under src/, compiled for the host (compile only at M0)
#   dread          the executable (main.cpp): the runtime now, the game objects from M1
#   sweep          tools/port_sweep.py with this build's compiler and target
# Configure from the repository root, for example
#   cmake -S port -B build/port/cmake -G Ninja
# port/README.md has the configure line for each platform, including the compile-only one for a
# Windows PC without a Windows SDK.
cmake_minimum_required(VERSION 3.25)
project(dread_port LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# One configuration by default, at the sweep's optimisation level (tools/port_sweep.py compiles
# with -O2): RelWithDebInfo is -O2 -g -DNDEBUG. An empty CMAKE_BUILD_TYPE would compile at -O0,
# which is not what the sweep measured.
get_property(_dread_multi_config GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(NOT _dread_multi_config AND NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Build type (default RelWithDebInfo: -O2 -g, the sweep's level)" FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Debug Release RelWithDebInfo MinSizeRel)
endif()

get_filename_component(DREAD_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/.." ABSOLUTE)
set(DREAD_GAME_VERSION "2.1.0" CACHE STRING "Game version the sources under src/ were matched against (passed to the sweep)")

# Linking needs a linker and a C/C++ runtime for the target. A try-compile that builds static
# libraries means the configuring machine has neither (a Windows PC without a Windows SDK), and
# a cross build may not either, so the executable is off by default in both cases.
if(CMAKE_TRY_COMPILE_TARGET_TYPE STREQUAL "STATIC_LIBRARY" OR CMAKE_CROSSCOMPILING)
  set(_dread_link_default OFF)
else()
  set(_dread_link_default ON)
endif()
option(DREAD_LINK "Build the dread executable (needs a linker and a C/C++ runtime for the target)" ${_dread_link_default})
option(DREAD_LINK_GAME "Link the game objects into dread (M1; fails until every function the boot path reaches has a source)" OFF)

# Standard library headers. On a Windows host without a Windows SDK the only usable C and C++
# headers are the ones the matching build uses: libc++ 9.0.1 (tools/fetch_sources.py puts it in
# %LOCALAPPDATA%\dread-decomp\sources) and musl (lib/musl), for a Linux target triple. When both
# variables are set the compile uses them with -nostdinc++, as tools/buildcfg.py does. Leave
# both empty for a real host build (Linux, macOS, Windows with an SDK): the system libc++ and
# libc are used and the result links.
set(_dread_libcxx_default "")
set(_dread_musl_default "")
if(CMAKE_HOST_WIN32 AND DEFINED ENV{LOCALAPPDATA})
  file(TO_CMAKE_PATH "$ENV{LOCALAPPDATA}" _dread_localappdata)
  set(_dread_libcxx_candidate "${_dread_localappdata}/dread-decomp/sources/libcxx-9.0.1.src/include")
  if(IS_DIRECTORY "${_dread_libcxx_candidate}" AND IS_DIRECTORY "${DREAD_ROOT}/lib/musl/include")
    set(_dread_libcxx_default "${_dread_libcxx_candidate}")
    set(_dread_musl_default "${DREAD_ROOT}/lib/musl/include")
  endif()
endif()
set(DREAD_LIBCXX_INCLUDE "${_dread_libcxx_default}" CACHE PATH "libc++ 9.0.1 include directory (empty: the host's own standard library)")
set(DREAD_MUSL_INCLUDE "${_dread_musl_default}" CACHE PATH "musl include directory (empty: the host's own C library)")

add_library(dread_stdlib INTERFACE)
target_compile_options(dread_stdlib INTERFACE -fno-rtti -fno-exceptions)
# CMAKE_POSITION_INDEPENDENT_CODE adds -fPIC only when CMake's platform is not Windows. The
# compile-only configure on a Windows host targets x86_64-linux-gnu while CMake's platform is
# still the host, so the flag is added by hand there; the sweep passes it too.
if(WIN32 AND CMAKE_CXX_COMPILER_TARGET AND NOT CMAKE_CXX_COMPILER_TARGET MATCHES "windows|msvc|mingw|cygwin")
  target_compile_options(dread_stdlib INTERFACE -fPIC)
endif()
if(DREAD_LIBCXX_INCLUDE AND DREAD_MUSL_INCLUDE)
  message(STATUS "dread: standard library headers from ${DREAD_LIBCXX_INCLUDE} and ${DREAD_MUSL_INCLUDE} (compile only)")
  target_compile_options(dread_stdlib INTERFACE -nostdinc++)
  target_include_directories(dread_stdlib SYSTEM INTERFACE "${DREAD_LIBCXX_INCLUDE}" "${DREAD_MUSL_INCLUDE}")
  target_compile_definitions(dread_stdlib INTERFACE _LIBCPP_HAS_MUSL_LIBC _LIBCPP_HAS_THREAD_API_PTHREAD)
elseif(DREAD_LIBCXX_INCLUDE OR DREAD_MUSL_INCLUDE)
  message(FATAL_ERROR "dread: set both DREAD_LIBCXX_INCLUDE and DREAD_MUSL_INCLUDE, or neither")
else()
  message(STATUS "dread: the host's own standard library")
endif()

add_subdirectory(runtime)

# The game: every source under src/, except the four below and anything matching
# DREAD_SWEEP_EXCLUDE: regexes over the repository-relative path, for sources the sweep reports
# as not yet portable. That list is empty because the first sweep (2026-10-24, port/README.md)
# found nothing to exclude.
#
# The Phase 3 identification samples (src/compiler_id, src/flag_id, src/atk_id; 36 frozen files)
# are compiled like everything else, except the four that define a function another source also
# defines, so that every function has one definition: fn_7100227448 (src/reflection/CActor.cpp
# defines the same symbol) and fn_71005a2dd8, fn_71006a3390, fn_71006de790 (a constructor here,
# a placeholder signature in src/lifted). The other 32 samples are the only definition of their
# function. tools/check.py exempts the sample directories from its DUPLICATE rule, so this list is
# kept current from the sweep's definitions report (tools/port_sweep.py reads the objects back
# with llvm-nm), not from check.py.
set(_dread_sample_duplicates
  src/flag_id/fn_7100227448.cpp
  src/flag_id/fn_71005a2dd8.cpp
  src/flag_id/fn_71006a3390.cpp
  src/flag_id/fn_71006de790.cpp)
set(DREAD_SWEEP_EXCLUDE "" CACHE STRING "Semicolon-separated CMake regexes over src/ paths to leave out of dread_game")
file(GLOB_RECURSE _dread_game_all CONFIGURE_DEPENDS "${DREAD_ROOT}/src/*.cpp")
set(_dread_game_sources "")
set(_dread_game_excluded "")
set(_dread_game_sample_dups "")
foreach(_src IN LISTS _dread_game_all)
  file(RELATIVE_PATH _rel "${DREAD_ROOT}" "${_src}")
  if(_rel IN_LIST _dread_sample_duplicates)
    list(APPEND _dread_game_sample_dups "${_rel}")
    continue()
  endif()
  set(_skip FALSE)
  foreach(_pattern IN LISTS DREAD_SWEEP_EXCLUDE)
    if(_rel MATCHES "${_pattern}")
      set(_skip TRUE)
      break()
    endif()
  endforeach()
  if(_skip)
    list(APPEND _dread_game_excluded "${_rel}")
  else()
    list(APPEND _dread_game_sources "${_src}")
  endif()
endforeach()
list(LENGTH _dread_game_sources _dread_game_count)
list(LENGTH _dread_game_excluded _dread_game_excluded_count)
list(LENGTH _dread_game_sample_dups _dread_game_sample_dups_count)
message(STATUS "dread: dread_game has ${_dread_game_count} sources, ${_dread_game_sample_dups_count} sample duplicates left out, ${_dread_game_excluded_count} excluded by DREAD_SWEEP_EXCLUDE")

# OBJECT, not STATIC: the whole-program link (M1) must keep every function, including the ones
# nothing references yet, and a static library would drop their objects.
add_library(dread_game OBJECT ${_dread_game_sources})
target_include_directories(dread_game PRIVATE "${DREAD_ROOT}/include")
target_link_libraries(dread_game PUBLIC dread_runtime)
# Matched code is held to the original's bytes, not to a newer compiler's warnings.
target_compile_options(dread_game PRIVATE -w)

if(DREAD_LINK)
  add_executable(dread main.cpp)
  target_link_libraries(dread PRIVATE dread_runtime)
  if(DREAD_LINK_GAME)
    target_link_libraries(dread PRIVATE dread_game)
  endif()
endif()

# The compile-only sweep, with this build's compiler, target and standard library choice.
if(CMAKE_HOST_WIN32)
  set(_dread_python_default "${DREAD_ROOT}/.venv/Scripts/python.exe")
else()
  set(_dread_python_default "${DREAD_ROOT}/.venv/bin/python")
endif()
if(NOT EXISTS "${_dread_python_default}")
  find_package(Python3 COMPONENTS Interpreter QUIET)
  if(Python3_Interpreter_FOUND)
    set(_dread_python_default "${Python3_EXECUTABLE}")
  else()
    set(_dread_python_default "python3")
  endif()
endif()
set(DREAD_PYTHON "${_dread_python_default}" CACHE FILEPATH "Python for tools/port_sweep.py (the repository's .venv when present)")
set(_dread_sweep_args --version "${DREAD_GAME_VERSION}" --cxx "${CMAKE_CXX_COMPILER}")
if(CMAKE_CXX_COMPILER_TARGET)
  list(APPEND _dread_sweep_args --target "${CMAKE_CXX_COMPILER_TARGET}")
endif()
if(NOT (DREAD_LIBCXX_INCLUDE AND DREAD_MUSL_INCLUDE))
  list(APPEND _dread_sweep_args --system-stdlib)
endif()
add_custom_target(sweep
  COMMAND "${DREAD_PYTHON}" "${DREAD_ROOT}/tools/port_sweep.py" ${_dread_sweep_args}
  WORKING_DIRECTORY "${DREAD_ROOT}"
  COMMENT "Compile-only sweep of src/ for the host (report in build/port/sweep.json)"
  USES_TERMINAL
  VERBATIM)
