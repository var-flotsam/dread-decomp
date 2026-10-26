// Constructor of a node (as in fn_71003771a8, src/game_s2/fn_71003771a8.cpp) over the base
// fn_71003944e8: sets the vtable, builds a runtime string from the empty string and names a string
// id, then finishes in vtable slot 0x18 of the node.
#include <new>

#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/UnkBehaviorTree.h"
#include "types.h"

using base::global::CRntString;
using base::global::UnkStrId;

struct UnkNode_71003a3e64 : UnkBehaviorTreeNode {
    CRntString field_0x38;
    UnkStrId field_0x68;

    void vf_0x18() { reinterpret_cast<void (*)(UnkNode_71003a3e64*)>(vtable[3])(this); }
};

extern void* g_unk_7101a28ac0[];

void fn_71003a3e64(UnkNode_71003a3e64* self) {
    fn_71003944e8(self);
    self->vtable = g_unk_7101a28ac0 + 2;
    new (&self->field_0x38) CRntString("");
    new (&self->field_0x68) UnkStrId("", true);
    self->vf_0x18();
}
