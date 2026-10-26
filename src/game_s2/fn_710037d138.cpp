// Constructor of an object derived from fn_7100393e68's: sets its vtable, clears its members and
// makes the two string ids at +0x50 and +0x58 the empty string (creating the pool first when there
// is none).
#include "base/global/CStrId.h"
#include "game/UnkBehaviorTree.h"
#include "types.h"

using base::global::UnkStringData;

struct UnkNode : UnkBehaviorTreeNodeBase {
    u8 pad_0x2c[4];
    u64 field_0x30;
    u64 field_0x38;
    u64 field_0x40;
    u8 pad_0x48[8];
    u64 field_0x50;
    UnkStringData* field_0x58;
};

extern void* g_unk_7101a25b90[];

void fn_710037d138(UnkNode* self) {
    fn_7100393e68(self);
    self->vtable = g_unk_7101a25b90 + 2;
    self->field_0x30 = 0;
    self->field_0x38 = 0;
    self->field_0x40 = 0;
    self->field_0x50 = 0;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->field_0x50 = (u64)&g_unk_7101d47108->empty;
    self->field_0x58 = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->field_0x58 = &g_unk_7101d47108->empty;
}
