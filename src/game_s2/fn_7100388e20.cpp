// Constructor of an object derived from fn_7100393e68's: sets its vtable, clears its members, gives
// a second vtable-bearing sub-object at +0x70 and makes the two string ids at +0x90 and +0x98 the
// empty string (creating the pool first when there is none).
#include "base/global/CStrId.h"
#include "game/UnkBehaviorTree.h"
#include "types.h"

using base::global::UnkStringData;

struct UnkNode : UnkBehaviorTreeNodeBase {
    u8 pad_0x2c[4];
    u64 field_0x30;
    u64 field_0x38;
    u64 field_0x40;
    u64 field_0x48;
    u8 field_0x50;
    u8 pad_0x51[3];
    u32 field_0x54;
    u64 field_0x58;
    u64 field_0x60;
    u8 field_0x68;
    u8 pad_0x69[7];
    void** field_0x70;
    u64 field_0x78;
    u64 field_0x80;
    u64 field_0x88;
    u64 field_0x90;
    UnkStringData* field_0x98;
};

extern void* g_unk_7101a265d0[];
extern void* g_unk_7101a22d48[];

void fn_7100388e20(UnkNode* self) {
    fn_7100393e68(self);
    self->vtable = g_unk_7101a265d0 + 2;
    self->field_0x30 = 0;
    self->field_0x38 = 0;
    self->field_0x40 = 0;
    self->field_0x48 = 0;
    self->field_0x50 = 0;
    self->field_0x54 = 0x7fffffff;
    self->field_0x58 = 0;
    self->field_0x60 = 0;
    self->field_0x68 = 0;
    self->field_0x70 = g_unk_7101a22d48 + 2;
    self->field_0x78 = 0;
    self->field_0x80 = 0;
    self->field_0x88 = 0;
    self->field_0x90 = 0;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->field_0x90 = (u64)&g_unk_7101d47108->empty;
    self->field_0x98 = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->field_0x98 = &g_unk_7101d47108->empty;
}
