// Collects the entries (0x30 bytes each, array at +0x48) whose key matches into `out`,
// first notifying the owner through a virtual for each entry with its flag set.
#include "base/UnkVector.h"
#include "types.h"

struct UnkStruct_71008651c8_Entry {
    u8 pad_0x0[0x8];
    void* field_0x8;
    u8 pad_0x10[0x8];
    bool field_0x18;
    u8 pad_0x19[0x17];
};
static_assert(sizeof(UnkStruct_71008651c8_Entry) == 0x30, "entry size");

using UnkOut_71008651c8 = base::UnkVector<UnkStruct_71008651c8_Entry*>;
UnkStruct_71008651c8_Entry** fn_7100865270(UnkOut_71008651c8* vec, u32 index);  // grows to fit

struct UnkStruct_71008651c8 {
    void** vtable;
    u8 pad_0x8[0x40];
    base::UnkVector<UnkStruct_71008651c8_Entry> field_0x48;
};

void fn_71008651c8(UnkStruct_71008651c8* self, void* const* key, UnkOut_71008651c8* out) {
    for (u32 i = 0; i < self->field_0x48.count; ++i) {
        UnkStruct_71008651c8_Entry* e = &self->field_0x48.data[i];
        if (e->field_0x8 != *key)
            continue;
        if (e->field_0x18)
            reinterpret_cast<void (*)(UnkStruct_71008651c8*, UnkStruct_71008651c8_Entry*, bool)>(
                self->vtable[0x170 / 8])(self, e, false);
        *fn_7100865270(out, out->count) = e;
    }
}
