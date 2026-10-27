#pragma once

// A component as the functions of the CComponent unit (tools/tu_map.py; the reflected size is 0x40)
// see it: fn_71010c4194 is a destructor (a 4-byte thunk to fn_710024f6d4, which stores the component
// vtable and runs the base destructor fn_7101043934) and
// fn_71010c4250 is the base property handler (it reads or writes a named setting of the component
// through a property record and falls back to fn_710024fc10 for unknown keys). The names and the
// layout are placeholders; the classes derived from it start their own members at 0x40.

#include "types.h"

struct UnkBaseComponent {
    void** vtable;
    u8 unk_0x8[0x28];
    void* field_0x30;  // an owner whose +0x28 member the property handler reads
    u8 unk_0x38[0x8];  // a flag byte at +0x3c is passed to the property stream
};
static_assert(sizeof(UnkBaseComponent) == 0x40, "UnkBaseComponent layout");

// The property record of fn_71010c4250: the name at +0x8, a stream at +0x38 and, at +0x78, whether
// the value is stored (true) or written out. The S7 handlers' own UnkProp (src/game_s7) is this
// record; they cast at the call.
struct UnkComponentProp;

// A destructor: a 4-byte thunk to fn_710024f6d4 (matching; the status is O, defined by the thunk
// only in generated code).
void fn_71010c4194(UnkBaseComponent* self);
// The base property handler; returns whether the key was handled.
u32 fn_71010c4250(UnkBaseComponent* self, UnkComponentProp* prop);
