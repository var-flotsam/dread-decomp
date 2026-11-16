#pragma once

// A reflected field as the tunables read it (fn_71000e76dc, fn_710013ab54): the byte offset of its
// value in the object, its type, and an optional accessor that reads the value instead. Names and
// layout are placeholders inferred from the code.
#include "types.h"

namespace base::reflection {
class CType;
}

struct UnkTunableFieldAccessor;

struct UnkTunableFieldHandle {
    u8 pad_0x0[8];
    u32 offset;
    const base::reflection::CType* type;
    UnkTunableFieldAccessor* accessor;  // null: the value is read at `offset`
};
