// Constructor of a node: runs its base constructor, sets the vtable, names two string ids with the
// empty string and finishes in vtable slot 0x18 of the node.
#include <new>

#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStrId;

typedef void** __attribute__((may_alias)) UnkVtable;

struct UnkNode {
    UnkVtable vtable;
    u8 pad_0x8[0x28];
    UnkStrId field_0x30;
    UnkStrId field_0x38;

    void vf_0x18() { reinterpret_cast<void (*)(UnkNode*)>(vtable[3])(this); }
};

void fn_7100393e68(UnkNode*);
extern void* g_unk_7101a25170[];

void fn_71003739b8(UnkNode* self) {
    fn_7100393e68(self);
    self->vtable = g_unk_7101a25170 + 2;
    new (&self->field_0x30) UnkStrId("", true);
    new (&self->field_0x38) UnkStrId("", true);
    self->vf_0x18();
}
