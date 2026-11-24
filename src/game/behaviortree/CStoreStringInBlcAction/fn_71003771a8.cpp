// Constructor of a node: runs its base constructor, sets the vtable, names a string id and builds a
// runtime string from the empty string, then finishes in vtable slot 0x18 of the node.
#include <new>

#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/UnkBehaviorTree.h"
#include "game/behaviortree/CStoreStringInBlcAction.h"
#include "types.h"

using base::global::CRntString;
using base::global::UnkStrId;

typedef void** UnkVtable;

struct UnkNode_3771a8 : UnkBehaviorTreeNodeBase {
    u8 pad_0x2c[4];
    UnkStrId field_0x30;
    CRntString sValue;

    void vf_0x18() { reinterpret_cast<void (*)(UnkNode_3771a8*)>(vtable[3])(this); }
};

void fn_71003771a8(UnkNode_3771a8* self) {
    fn_7100393e68(self);
    self->vtable = g_unk_7101a25640 + 2;
    new (&self->field_0x30) UnkStrId("", true);
    new (&self->sValue) CRntString("");
    self->vf_0x18();
}
