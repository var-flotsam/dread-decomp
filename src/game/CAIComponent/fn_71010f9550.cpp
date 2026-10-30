// The CAIComponent unit's copy of the string list's destructor (base/UnkNodeOwner.h): nodes linked at
// 0x20 and destroyed by fn_71002874dc, and this unit's two vtables.
#include "base/UnkEmptyDtorElem.h"
#include "base/UnkNodeOwner.h"

extern void* g_unk_7101cb25f8[];  // derived part's vtable
extern char unk_7101cb2618[];     // base part's vtable (src/game_s1/fn_7100219134.cpp)

template <u32 Next>
__attribute__((always_inline)) inline void UnkDestroyEmptyNode(UnkListNode<Next>* node) {
    fn_71002874dc(node);
}

void fn_71010f9550(UnkNodeOwner<UnkListNode<0x20>, UnkDestroyEmptyNode<0x20>>* self) {
    self->Destroy(g_unk_7101cb25f8, reinterpret_cast<void**>(unk_7101cb2618));
}
