// A copy of fn_71002f2974 (base/UnkNodeOwner.h) in the CCharClassFrozenAsPlatformComponent unit: nodes
// linked at 0xc8, freed with their text, and this unit's two vtables.
#include "base/UnkNodeOwner.h"

extern void* g_unk_7101a74458[];  // derived part's vtable
extern void* g_unk_7101a74478[];  // base part's vtable

void fn_71007fd1dc(UnkNodeOwner<UnkListNode<0xc8>, UnkDestroyStrNode<0xc8>>* self) {
    self->Destroy(g_unk_7101a74458, g_unk_7101a74478);
}
