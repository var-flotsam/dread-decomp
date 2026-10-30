// A copy of fn_71002f2974 (base/UnkNodeOwner.h) in the CMeleeComponent unit: nodes linked at 0x88,
// freed with their text, and this unit's two vtables.
#include "base/UnkNodeOwner.h"

extern void* g_unk_7101a838c8[];  // derived part's vtable
extern void* g_unk_7101a838e8[];  // base part's vtable

void fn_7100931c34(UnkNodeOwner<UnkListNode<0x88>, UnkDestroyStrNode<0x88>>* self) {
    self->Destroy(g_unk_7101a838c8, g_unk_7101a838e8);
}
