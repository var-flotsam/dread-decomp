// Constructor: runs the base constructor, sets the vtable and builds two records of the same
// layout at 0x38 and 0x50: an empty string id (creating the pool first when there is none) followed
// by a cleared word and a cleared 32-bit value.
#include <new>

#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;

struct UnkEntry {
    UnkStringData* data;
    u64 field_0x8;
    u32 field_0x10;
    u8 pad_0x14[4];

    UnkEntry() {
        data = nullptr;
        if (!g_unk_7101d47108)
            base::global::UnkEnsureStringPool();
        data = &g_unk_7101d47108->empty;
        field_0x8 = 0;
        field_0x10 = 0;
    }
};

struct UnkNode {
    void** vtable;
    u8 pad_0x8[0x30];
    UnkEntry field_0x38;
    UnkEntry field_0x50;
};

void fn_71003944e8(UnkNode* self);
extern void* g_unk_7101a27d40[];

void fn_710039d420(UnkNode* self) {
    fn_71003944e8(self);
    self->vtable = g_unk_7101a27d40 + 2;
    new (&self->field_0x38) UnkEntry();
    new (&self->field_0x50) UnkEntry();
}
