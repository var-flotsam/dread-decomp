// Copies of fn_71002f2974 (src/game/CPlaythroughDef/fn_71002f2974.cpp; base/UnkNodeOwner.h) for other
// node types, naming their own vtables: fn_71000fc7cc (nodes linked at 0x148), fn_71000732d4 (nodes
// linked at 0x30 and destroyed by fn_710007ecd4 alone).
#include "base/UnkNodeOwner.h"

extern void* g_unk_7101a1a070[];  // base part's vtable (fn_71000732d4)

void fn_71000fc7cc(UnkNodeOwner<UnkListNode<0x148>, UnkDestroyStrNode<0x148>>* self) {
    self->Destroy(g_unk_7101a1bcd0, g_unk_7101a1bcf0);
}

void fn_71000732d4(UnkStrList30* self) {
    self->Destroy(g_unk_7101a1a4a8, g_unk_7101a1a070);
}
