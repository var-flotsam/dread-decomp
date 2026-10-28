// Constructor of a small record: an empty string id followed by a null/zero 8-byte field.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkStruct_71002ef5bc {
    CStrId field_0x0;
    u64 field_0x8;
};

void fn_71002ef5bc(UnkStruct_71002ef5bc* self) {
    // The id's default constructor, inlined.
    self->field_0x0.data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->field_0x0.data = &g_unk_7101d47108->empty;
    self->field_0x8 = 0;
}
