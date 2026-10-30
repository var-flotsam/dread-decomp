// The CFXComponent unit's copy of fn_71011b250c (src/game/GUI/CGUIManager.cpp): the constructor of a
// container of 0x18-byte nodes over an embedded block pool (base/UnkParentPoolList.h), with this
// unit's two vtables.
#include "base/UnkParentPoolList.h"

extern char unk_7101cb38e8[];  // the base part's vtable (reached through the GOT)
extern char unk_7101cb38c8[];  // the container's vtable (reached through the GOT)

void fn_71011571f8(UnkParentPoolList* self, u32 n, u32 /*unused*/, u32 flags, base::UnkAllocator* a) {
    refl_ParentPoolListCtor<0x18, unk_7101cb38e8, unk_7101cb38c8>(self, n, flags, a);
}
