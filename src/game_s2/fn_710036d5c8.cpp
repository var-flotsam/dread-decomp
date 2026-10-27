// Constructor of a node: runs its base constructor, sets the vtable, initialises its fields and
// names its string ids, then runs the post-construction virtual call.
#include <new>

#include "base/global/CStrId.h"
#include "game/UnkBehaviorTree.h"
#include "types.h"

using base::global::UnkStrId;

template <class T>
struct __attribute__((packed)) UnkPacked {
    T v;
};

struct UnkNode : UnkBehaviorTreeNodeBase {
    UnkStrId field_0x30;
    UnkStrId field_0x38;
    u32 field_0x40;
    UnkPacked<u64> field_0x44;
    u32 field_0x4c;
    UnkStrId field_0x50;
    UnkStrId field_0x58;
    u8 field_0x60;
};

extern void* g_unk_7101a24ba8[];

void fn_710036d5c8(UnkNode* self) {
    fn_7100393e68(self);
    self->vtable = &g_unk_7101a24ba8[2];
    new (&self->field_0x30) UnkStrId("", true);
    new (&self->field_0x38) UnkStrId("", true);
    self->field_0x40 = 0x1000000;
    self->field_0x44.v = -1;
    self->field_0x4c = -1;
    new (&self->field_0x50) UnkStrId("", true);
    new (&self->field_0x58) UnkStrId("", true);
    self->field_0x60 = 0;
    reinterpret_cast<void (*)(UnkNode*)>(self->vtable[3])(self);
}
