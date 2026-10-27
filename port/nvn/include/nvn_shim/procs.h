#pragma once

// The NVN shim's entry layer: how the game finds the shim's functions. The graphics start-up
// (fn_71001587f0) asks the bootstrap loader for nvnDeviceGetProcAddress, then the loader
// fn_7100154770 asks that for all 523 names it knows and stores each pointer in a global (twice:
// once with no device, once with the device). Each name is a string in the game; the global each
// lands in is listed in data/nvn_procs.csv (tools/nvn_procs.py).
//
// Every name gets a pointer. The shim's implemented functions are registered by name
// (RegisterProc, from each part's Register...Procs(), which the first lookup calls). A name without an
// implementation gets one of a pool of trap thunks bound to it: called, the trap reports the name
// once on stderr and returns 0, so a run lists the NVN functions the game reaches that the shim
// lacks, in the order it reaches them (like dread_port_missing_function for game code). The 0 is an
// integer return: a function returning a float or a structure in memory would see garbage, so the
// list is for finding the next functions to write, not for running past them.

#include <cstddef>
#include <string>
#include <vector>

extern "C" void* nvnBootstrapLoader(const char* name);

namespace dread::nvn {

using Proc = void (*)();

// The shim's pointer for an NVN function name: its implementation, or a trap bound to the name.
// Null only for a null name.
Proc GetProc(const char* name);

// Registers an implementation. A second registration of one name keeps the first and is reported
// by DuplicateRegistrations().
bool RegisterProc(const char* name, Proc proc);

template <class F>
bool Register(const char* name, F* fn) {
    return RegisterProc(name, reinterpret_cast<Proc>(fn));
}

// For checks and diagnostics.
std::vector<std::string> Registered();       // implemented names, sorted
std::vector<std::string> TrapsCalled();      // names whose trap ran, in the order first called
std::vector<std::string> DuplicateRegistrations();

}  // namespace dread::nvn
