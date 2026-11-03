// CCharClassGrabComponent unit (tools/tu_map.py, reflection hint). fn_7100790e78: the deleting
// destructor; the body of fn_7100790da8 (CCharClassGrabComponent_b43.cpp) followed by operator delete.
// Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

extern void* g_unk_7101a6f508[];
extern void* g_unk_7101a6f558[];
// Signatures as src/game/CCharClassCollisionComponent/CCharClassCollisionComponent.cpp and
// src/game/CUsableComponent/fn_7100a9e6bc.cpp declare them.
struct UnkIdObject;
void fn_71011acd74(UnkIdObject* self);
void fn_7100431cec(void* self);

struct UnkListNode_grab_b49a3 {
    u8 pad_0x0[0x88];
    UnkListNode_grab_b49a3* next;  // 0x88
};

struct UnkList_grab_b49a3 {
    void** vtable;  // 0x58
    u32 count;  // 0x60
    UnkListNode_grab_b49a3* head;  // 0x68
    UnkListNode_grab_b49a3* tail;  // 0x70
    base::UnkAllocator* allocator;  // 0x78
};

struct UnkGrabComponent_b49a3 {
    void** vtable;
    u8 pad_0x8[0x50];
    UnkList_grab_b49a3 list;  // 0x58
    u8 pad_0x80[0x8];
    UnkStringData* id;  // 0x88, a CStrId
};
static_assert(__builtin_offsetof(UnkGrabComponent_b49a3, id) == 0x88, "UnkGrabComponent_b49a3 layout");

void fn_7100790e78(UnkGrabComponent_b49a3* self) {
    self->vtable = &g_unk_7101a6f508[2];
    if (UnkStringPool* pool = g_unk_7101d47108) {
        UnkStringData** id = &self->id;
        if (*id && *id != &pool->empty) {
            --(*id)->ref_count;
            if ((*id)->ref_count == 0) {
                fn_71000803a4(pool, *id);
                pool = g_unk_7101d47108;
            }
        }
        *id = &pool->empty;
    }
    UnkList_grab_b49a3* list = &self->list;
    list->vtable = &g_unk_7101a6f558[2];
    for (UnkListNode_grab_b49a3* node = list->head; node;) {
        UnkListNode_grab_b49a3* next = node->next;
        fn_7100431cec(node);
        list->allocator->vf_0x48(node);
        node = next;
    }
    list->count = 0;
    list->head = nullptr;
    list->tail = nullptr;
    fn_71011acd74(reinterpret_cast<UnkIdObject*>(self));
    operator delete(self);
}
