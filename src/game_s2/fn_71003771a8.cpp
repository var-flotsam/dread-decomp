// Constructor of a node: runs its base constructor, sets the vtable, names a string id and builds a
// runtime string from the empty string, then finishes in vtable slot 0x18 of the node.
#include <new>

#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/UnkBehaviorTree.h"
#include "types.h"

using base::global::CRntString;
using base::global::UnkStrId;

typedef void** __attribute__((may_alias)) UnkVtable;

struct UnkNode : UnkBehaviorTreeNodeBase {
    u8 pad_0x2c[4];
    UnkStrId field_0x30;
    CRntString field_0x38;

    void vf_0x18() { reinterpret_cast<void (*)(UnkNode*)>(vtable[3])(this); }
};

extern void* g_unk_7101a25640[];

void fn_71003771a8(UnkNode* self) {
    fn_7100393e68(self);
    self->vtable = g_unk_7101a25640 + 2;
    new (&self->field_0x30) UnkStrId("", true);
    new (&self->field_0x38) CRntString("");
    self->vf_0x18();
}
