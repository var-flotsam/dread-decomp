#pragma once

// String ids in two parallel arrays, as fn_71004f11f8 sees them: 16-byte values whose first word
// is a string id, and the string-id keys, with a shared count (+0x10) and the allocator at +0x20.
// tools/tu_map.py puts fn_71004f11f8 in the unit it names after CEmmyCaveAIComponent. The names
// and the layout are placeholders.

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

struct UnkStrIdTableValue {
    base::global::CStrId id;
    u64 field_0x8;
};

struct UnkStrIdTable {
    UnkStrIdTableValue* values;
    base::global::CStrId* keys;
    u32 count;
    u8 unk_0x14[0xc];
    base::UnkAllocator* allocator;
};
static_assert(sizeof(UnkStrIdTable) == 0x28, "UnkStrIdTable placeholder layout");
void fn_71004f11f8(UnkStrIdTable* self);
