// A copy of fn_71002f2974 (base/UnkNodeOwner.h) in the CCharClassFrozenAsPlatformComponent unit: the
// destructor of the string-piece list (game/UnkStrIdSplit.h), nodes linked at 0xc8 and freed with
// their text, and this unit's two vtables.
#include "base/UnkNodeOwner.h"
#include "game/UnkStrIdSplit.h"

extern void* g_unk_7101a74478[];  // base part's vtable

void fn_71007fd1dc(UnkStrPieceList* self) {
    using List = UnkNodeOwner<UnkListNode<0xc8>, UnkDestroyStrNode<0xc8>>;
    reinterpret_cast<List*>(self)->Destroy(reinterpret_cast<void**>(g_unk_7101a74458), g_unk_7101a74478);
}
