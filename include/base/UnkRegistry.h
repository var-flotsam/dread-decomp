#pragma once

// The 0x70-byte registry that the GUI script loader (src/game_hi/fn_71011b69d8.cpp) creates on
// first use. Its vtable is at GOT 0x7101cbc3d8; the layout is inferred from the constructor
// fn_7100087388 and the names are placeholders: an allocator and flags word (0xe), a pointer
// vector (base/UnkVector.h, flags 0xe, reserved to 11 entries through its own allocator) and six
// zeroed words.

#include "base/UnkVector.h"
#include "types.h"

struct UnkRegistry_0x70 {
    void* vtable;
    base::UnkAllocator* allocator;  // +0x8, fn_7100083990
    u32 flags;                      // +0x10, 0xe
    u8 pad_0x14[4];
    base::UnkVector<void*> entries;  // +0x18
    u64 field_0x40;
    u64 field_0x48;
    u64 field_0x50;
    u64 field_0x58;
    u64 field_0x60;
    u64 field_0x68;
};
static_assert(sizeof(UnkRegistry_0x70) == 0x70, "UnkRegistry_0x70 placeholder layout");

// Its constructor (a plain function: callers pass the object they allocated).
// Defined in src/base/UnkRegistry.cpp.
void fn_7100087388(UnkRegistry_0x70* self);

// Whether the registry has an entry for an id and path pair (base/UnkStrIdPathPair.h; its vf_0x20
// looks it up). Defined in src/base/UnkRegistry.cpp.
struct UnkStrPair_0x10;
bool fn_7100087850(UnkRegistry_0x70* self, const UnkStrPair_0x10* key);
