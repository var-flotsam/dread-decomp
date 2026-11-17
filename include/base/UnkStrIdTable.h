#pragma once

// String ids in two parallel arrays: 16-byte values whose first word is a string id, and the
// string-id keys, with a shared count (+0x10; what fn_71004f11f8 reads) and, from its callers'
// layouts (fn_71004f0428), the allocator at +0x20.
// tools/tu_map.py puts fn_71004f11f8 in the unit it names after CEmmyCaveAIComponent. The names
// and the layout are placeholders.

#include "base/global/CRntVector.h"
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

// The table as a member owns it: destroyed by resetting its ids (fn_71004f11f8), then freeing the keys and
// the values (CSpawnPointComponent's destructor fn_7101125c90; the destructors of other classes still
// write the same sequence with a table type of their own).
struct UnkOwnedStrIdTable;

// Resets every id in both arrays to the empty id and clears the count. Not decompiled yet.
void fn_71004f11f8(UnkStrIdTable* self);

struct UnkOwnedStrIdTable : UnkStrIdTable {
    ~UnkOwnedStrIdTable() {
        fn_71004f11f8(this);
        if (keys)
            allocator->vf_0x48(keys);
        if (values)
            allocator->vf_0x48(values);
    }
};
static_assert(sizeof(UnkOwnedStrIdTable) == 0x28, "UnkOwnedStrIdTable layout");
