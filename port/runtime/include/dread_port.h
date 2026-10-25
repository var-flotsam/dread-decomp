#pragma once

// The port runtime's own surface, as opposed to the nn:: declarations under nn/: what the link's
// generated inputs (tools/port_link.py) and the port's own code call. Nothing here stands in for an
// SDK function.

#include "types.h"

// Where a call to a function the port has no source for ends up. tools/port_link.py writes one jump
// thunk per such function (build/port/link/traps.s) that loads the function's original address and
// jumps here; the runtime prints the address, to look up in data/functions/, and aborts, so a boot
// that reaches unported code stops at the call instead of running off into nothing.
extern "C" [[noreturn]] void dread_port_missing_function(u64 address);

// Where a call through a declaration the link refused to alias ends up: the function is defined, but
// the caller's parameter list puts an argument in a different register class than the definition
// reads (a float where the definition takes a bool: class_mismatch in the link report), so a call
// would compute with the wrong register. The thunk loads the function's address and jumps here;
// fixing the declaration (tools/decl_check.py) removes the thunk.
extern "C" [[noreturn]] void dread_port_mismatched_call(u64 address);
