// Constructor of an object derived from fn_710041a198's: sets its vtable, makes the two string ids
// at +0x538 and +0x558 the empty string (creating the pool first when there is none) and sets a few
// flags.
#include "base/global/CStrId.h"
#include "game/UnkBossAIComponent.h"
#include "types.h"

using base::global::UnkStringData;

struct UnkNode : UnkBossAIComponent {
    u8 pad_0x528[0x10];
    u64 field_0x538;
    u8 pad_0x540[0x18];
    UnkStringData* field_0x558;
};

extern void* g_unk_7101a3fb70[];

void fn_71004d9014(UnkNode* self) {
    fn_710041a198(self);
    self->vtable = g_unk_7101a3fb70 + 2;
    self->field_0x538 = 0;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->field_0x538 = (u64)&g_unk_7101d47108->empty;
    self->field_0x558 = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->field_0x558 = &g_unk_7101d47108->empty;
    self->field_0x40b = 1;
    self->field_0x4fc = 0x101;
    self->field_0x409 = 1;
}
