#pragma once

// The port runtime's own surface, as opposed to the nn:: declarations under nn/: what the link's
// generated inputs (tools/port_link.py) and the port's own code call. Nothing here stands in for an
// SDK function.

#include "types.h"

// Where a call to a function the port has no source for ends up. tools/port_link.py writes one jump
// thunk per such function (build/port/link/traps.s) that loads the function's original address and
// jumps here; the runtime prints the address, to look up in data/functions/, and aborts, so a boot
// that reaches unported code stops at the call instead of running off into nothing.
//
// With DREAD_TRAP=log in the environment the three traps here report each function or import once
// and return 0 to the caller of the thunk instead (the thunk jumps, so the return goes straight
// back): a run then lists, in the order the game reaches them, every missing function until
// something truly breaks. For finding the boot's work list; the values the game computes from those
// zeros are wrong. That is why the traps return a word and are not [[noreturn]].
extern "C" u64 dread_port_missing_function(u64 address);

// Where a call through a declaration the link refused to alias ends up: the function is defined, but
// the caller's parameter list puts an argument in a different register class than the definition
// reads (a float where the definition takes a bool: class_mismatch in the link report), so a call
// would compute with the wrong register. The thunk loads the function's address and jumps here;
// fixing the declaration (tools/decl_check.py) removes the thunk.
extern "C" u64 dread_port_mismatched_call(u64 address);

// The port's main passes its own arguments before it calls into the game: nn::os::GetHostArgc and
// GetHostArgv return them (nnMain's first call, fn_71000005d0, walks them). Without a call both
// report no arguments.
extern "C" void dread_port_set_host_arguments(int argc, char** argv);

// Asks the game to exit as the console's system does: the next nn::oe::TryPopNotificationMessage
// returns the exit request (4), on which the notification loop fn_71001327c0 sets the quit flag.
// For the port's window (close button) and console (Ctrl+C); safe from any thread.
extern "C" void dread_port_request_exit();

// Where a game pointer to an import the port has no definition for ends up (a GOT or PLT slot, a
// vtable entry naming an SDK function): the data loader writes a thunk that loads the import's
// name and jumps here (dread_port_loader.h).
extern "C" u64 dread_port_missing_import(const char* name);

// Runs the game's static initialisers as the SDK does before nnMain: main's .init_array in order
// (tools/port_link.py's dread_port_initialisers), each through the port's definition. One without a
// definition is skipped and counted (DREAD_INIT=list names each); its unit's globals stay as the
// dump has them. The port's main calls it right before nnMain.
extern "C" void dread_port_run_game_initialisers();

// Maps the game's main NSO from the user's dump at its original addresses (dread_port_loader.h).
// Runs by itself before the game's static initialisers when the game is linked in; aborts with a
// message if the dump is missing or not the expected build.
extern "C" void dread_port_map_game_image();

// The game's memory block (nn::os::AllocateMemoryBlock, which nninitStartup asks for and gives to
// the default allocator): whether it exists yet, and whether a pointer lies in it. The port's C++
// heap (src/heap.cpp) allocates from the game's allocator once it exists and frees each pointer
// with the allocator it came from.
extern "C" bool dread_port_game_heap_ready();
extern "C" bool dread_port_in_game_heap(const void* ptr);

// The user's dump, laid out as orig/<version>/ (exefs/, romfs/): $DREAD_ORIG, else orig/2.1.0
// under the working directory. The data loader reads exefs/main from it, nn::fs mounts romfs/ as
// "rom".
extern "C" const char* dread_port_orig_directory();

// The state of the port's one controller (nn::hid's handheld Npad, id 0x20), set by the port's
// window from the keyboard or a host game controller; the game's poll fn_710006deac reads it every
// frame through nn::hid::GetNpadState. Buttons in the console's bit order; sticks from -32767 to
// 32767. Safe from any thread.
extern "C" void dread_port_set_npad_state(u64 buttons, s32 left_x, s32 left_y, s32 right_x, s32 right_y);
