// Constructor of a node: runs its base constructor, sets the vtable, initialises its fields and
// names its string ids, then runs the post-construction virtual call.
#include <new>

#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStrId;

struct UnkNode {
    void** vtable;
    u8 pad_0x8[0x24];
    u32 field_0x2c;
    UnkStrId field_0x30;
};

void fn_7100393e68(UnkNode*);
extern void* g_unk_7101a24f60[];

void fn_7100372124(UnkNode* self) {
    fn_7100393e68(self);
    self->vtable = &g_unk_7101a24f60[2];
    self->field_0x2c = 6;
    new (&self->field_0x30) UnkStrId("", true);
    reinterpret_cast<void (*)(UnkNode*)>(self->vtable[3])(self);
}
