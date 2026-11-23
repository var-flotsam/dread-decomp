// Adds `child` to the owner's list of children: the child's weak reference at +0x20 is made to refer to
// the owner's weak handle (created on first use), then the child is appended to the array at +0x30.
#include "base/global/CRntVector.h"
#include "game_s2/UnkWeakHandle.h"

struct UnkChild_396290 {
    u8 pad_0x0[0x20];
    UnkWeakHandle* field_0x20;
};

struct UnkOwner_396290 : UnkWeakTarget {
    u8 pad_0x8[0x28];
    base::global::CRntVector<UnkChild_396290*> field_0x30;
};

void fn_7100396290(UnkOwner_396290* self, UnkChild_396290* child) {
    UnkWeakHandle* handle = self ? GetWeakHandle(self) : nullptr;
    AssignRef(&child->field_0x20, handle);
    self->field_0x30.PushBack(child);
}
