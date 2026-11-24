// CCharClassGrabComponent unit (tools/tu_map.py, reflection hint). fn_7100790da8: a destructor body:
// the vtable, the string id at +0x88 reset to the pool's empty id (released when its count drops to
// zero), then the list member at +0x58 (its vtable, then each node of the list at +0x68 destroyed by
// fn_7100431cec and freed through the allocator at +0x78; the count at +0x60 and both ends cleared)
// and the base destructor fn_71011acd74. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CCharClassComponent.h"
#include "game/CCharClassGrabComponent.h"
#include "game/CChozoCommanderKiGrabAttack.h"
#include "types.h"
#include "base/UnkVtableList.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

// Signatures as src/game/CCharClassCollisionComponent/CCharClassCollisionComponent.cpp and
// src/game/CUsableComponent/fn_7100a9e6bc.cpp declare them.
struct UnkIdObject_8da5c0;

struct UnkListNode_grab_b43a2 {
    u8 pad_0x0[0x88];
    UnkListNode_grab_b43a2* next;  // 0x88
};

struct UnkGrabComponent_b43a2 {
    void** vtable;
    u8 pad_0x8[0x50];
    base::UnkVtableList<UnkListNode_grab_b43a2> list;  // 0x58
    UnkStringData* id;  // 0x88, a CStrId
};
static_assert(__builtin_offsetof(UnkGrabComponent_b43a2, id) == 0x88, "UnkGrabComponent_b43a2 layout");

void CCharClassGrabComponent_Destroy(UnkGrabComponent_b43a2* self) {
    self->vtable = &g_unk_7101a6f508[2];
    if (UnkStringPool* pool = g_unk_7101d47108) {
        UnkStringData** id = &self->id;
        if (*id && *id != &pool->empty) {
            --(*id)->ref_count;
            if ((*id)->ref_count == 0) {
                StringPool_Release(pool, *id);
                pool = g_unk_7101d47108;
            }
        }
        *id = &pool->empty;
    }
    base::UnkVtableList<UnkListNode_grab_b43a2>* list = &self->list;
    list->vtable = &g_unk_7101a6f558[2];
    for (UnkListNode_grab_b43a2* node = list->head; node;) {
        UnkListNode_grab_b43a2* next = node->next;
        fn_7100431cec(node);
        list->allocator->vf_0x48(node);
        node = next;
    }
    list->count = 0;
    list->head = nullptr;
    list->tail = nullptr;
    fn_71011acd74(reinterpret_cast<UnkIdObject_8da5c0*>(self));
}
