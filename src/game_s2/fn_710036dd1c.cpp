// Constructor of a node: runs its base constructor, sets the vtable, names two string ids,
// leaves a third as the empty string and sets three flag bytes.
#include <new>

#include "base/global/CStrId.h"
#include "game/UnkBehaviorTree.h"
#include "types.h"

using base::global::UnkStrId;
using base::global::UnkStrIdBase;

struct UnkNode : UnkBehaviorTreeNodeBase {
    UnkStrId field_0x30;
    UnkStrId field_0x38;
    UnkStrIdBase field_0x40;
    bool field_0x48;
    bool field_0x49;
    bool field_0x4a;
};

extern void* g_unk_7101a24c58[];

void fn_710036dd1c(UnkNode* self) {
    fn_7100393e68(self);
    self->vtable = &g_unk_7101a24c58[2];
    new (&self->field_0x30) UnkStrId("", true);
    new (&self->field_0x38) UnkStrId("", true);
    self->field_0x40.data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->field_0x40.data = &g_unk_7101d47108->empty;
    self->field_0x48 = false;
    self->field_0x49 = true;
    self->field_0x4a = false;
}
