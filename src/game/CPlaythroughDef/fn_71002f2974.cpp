// The destructor of a string list with an embedded block pool (base/UnkNodeOwner.h): nodes linked at
// 0x68, freed with their text.
#include "base/UnkNodeOwner.h"

extern void* g_unk_7101a222a8[];  // derived part's vtable
extern void* g_unk_7101a222c8[];  // base part's vtable

void fn_71002f2974(UnkNodeOwner<UnkListNode<0x68>, UnkDestroyStrNode<0x68>>* self) {
    self->Destroy(g_unk_7101a222a8, g_unk_7101a222c8);
}
