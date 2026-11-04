// tools/tu_map.py unit 852 (no class hint): the destructor of a node owner (base/UnkNodeOwner.h) whose
// nodes hold a refcounted pointer at +0 and the next link at +8 (vtables 0x7101a4d8f8 and 0x7101a4d918).
// Names are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkNodeOwner.h"
#include "types.h"

extern void* g_unk_7101a4d8f8[];
extern void* g_unk_7101a4d918[];

__attribute__((always_inline)) inline void UnkDestroyRefNode_A3_b69(UnkListNode<8>* node) {
    reinterpret_cast<base::UnkRefPtrMember*>(node)->~UnkRefPtrMember();
}

using UnkRefNodeOwner_A3_b69 = UnkNodeOwner<UnkListNode<8>, UnkDestroyRefNode_A3_b69>;

void fn_710059c7b8(UnkRefNodeOwner_A3_b69* self) {
    self->Destroy(g_unk_7101a4d8f8, g_unk_7101a4d918);
}
