// Constructor of an object derived from fn_7100393e68's: sets its vtable, clears its members, sets a
// flag and makes the two string ids at +0x48 and +0x50 the empty string (creating the pool first
// when there is none).
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;

struct UnkNode {
    void** vtable;
    u8 pad_0x8[0x28];
    u8 field_0x30[0xb];
    u8 field_0x3b;
    u8 pad_0x3c[4];
    u32 field_0x40;
    u8 pad_0x44[4];
    u64 field_0x48;
    UnkStringData* field_0x50;
};

void fn_7100393e68(UnkNode*);
extern void* g_unk_7101a267e0[];

void fn_710038db9c(UnkNode* self) {
    fn_7100393e68(self);
    self->vtable = g_unk_7101a267e0 + 2;
    __builtin_memset(self->field_0x30, 0, 11);
    self->field_0x3b = 1;
    self->field_0x40 = 0;
    self->field_0x48 = 0;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->field_0x48 = (u64)&g_unk_7101d47108->empty;
    self->field_0x50 = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->field_0x50 = &g_unk_7101d47108->empty;
}
