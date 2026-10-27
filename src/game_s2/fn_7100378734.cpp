// Constructor of a node: runs its base constructor, sets the vtable, initialises its fields and
// names its string ids, then runs the post-construction virtual call.
#include <new>

#include "base/global/CStrId.h"
#include "game/UnkBehaviorTree.h"
#include "types.h"

using base::global::UnkStrId;

struct UnkNode : UnkBehaviorTreeNodeBase {
    u8 pad_0x2c[4];
    UnkStrId field_0x30;
    u32 field_0x38;
    u8 pad_0x3c[0x4];
    u64 field_0x40;
};

extern void* g_unk_7101a25850[];

void fn_7100378734(UnkNode* self) {
    fn_7100393e68(self);
    self->vtable = &g_unk_7101a25850[2];
    new (&self->field_0x30) UnkStrId("", true);
    self->field_0x38 = 0x10101;
    self->field_0x40 = 0;
    reinterpret_cast<void (*)(UnkNode*)>(self->vtable[3])(self);
}
