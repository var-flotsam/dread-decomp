// Constructor of a node: runs its base constructor, sets the vtable, initialises its fields and
// names its string ids, then runs the post-construction virtual call.
#include <new>

#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStrId;

struct UnkNode {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkStrId field_0x30;
    u8 field_0x38;
};

void fn_7100393e68(UnkNode*);
extern void* g_unk_7101a259b0[];

void fn_7100379784(UnkNode* self) {
    fn_7100393e68(self);
    self->vtable = &g_unk_7101a259b0[2];
    new (&self->field_0x30) UnkStrId("", true);
    self->field_0x38 = 1;
    reinterpret_cast<void (*)(UnkNode*)>(self->vtable[3])(self);
}
