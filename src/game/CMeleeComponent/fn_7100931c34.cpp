// A copy of fn_71002f2974 (base/UnkNodeOwner.h) in the CMeleeComponent unit: the destructor of the
// token list src/game/GUI/CDisplayObjectContainer/fn_71011d9508.cpp splits a path into, nodes linked
// at 0x88 and freed with their text, and this unit's two vtables.
#include "base/UnkNodeOwner.h"

struct UnkTokenList_0x70;
extern char g_unk_7101a838c8[];   // derived part's vtable
extern void* g_unk_7101a838e8[];  // base part's vtable

void fn_7100931c34(UnkTokenList_0x70* list) {
    using List = UnkNodeOwner<UnkListNode<0x88>, UnkDestroyStrNode<0x88>>;
    reinterpret_cast<List*>(list)->Destroy(reinterpret_cast<void**>(g_unk_7101a838c8), g_unk_7101a838e8);
}
