// CRodotukSuckAttack unit (tools/tu_map.py unit 990). Its destructors: the vtable, the member at +0x70
// (fn_7100d62bd8), then the list at +0x28 (its vtable, then each node, which holds a counted reference
// and the next node, released and freed through the allocator at +0x48; the count and both ends
// cleared) and CAttack's destructor (fn_71003606c0); fn_710066a4b0 also frees the object. Names and
// layouts are placeholders.
#include "base/UnkVector.h"
#include "../../game_5821e4/CAttackDtor.h"
#include "game/CRodotukSuckAttack.h"
#include "game/logic/collision/CPolygonCollectionShape.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkRefNode_b49a3 {
    UnkRefObject* ref;
    UnkRefNode_b49a3* next;
};

struct UnkRefList_b49a3 {
    void** vtable;  // 0x28
    u32 count;  // 0x30
    UnkRefNode_b49a3* head;  // 0x38
    UnkRefNode_b49a3* tail;  // 0x40
    base::UnkAllocator* allocator;  // 0x48
};

struct UnkRodotukSuckAttack_b49a3 {
    void** vtable;
    u8 pad_0x8[0x20];
    UnkRefList_b49a3 list;  // 0x28
    u8 pad_0x50[0x20];
    u8 field_0x70[0x8];
};

__attribute__((always_inline)) static inline void Destroy_b49a3(UnkRodotukSuckAttack_b49a3* self) {
    self->vtable = g_unk_7101a5da10 + 2;
    fn_7100d62bd8(self->field_0x70);
    UnkRefList_b49a3* list = &self->list;
    list->vtable = g_unk_7101a5db18 + 2;
    for (UnkRefNode_b49a3* node = list->head; node;) {
        UnkRefNode_b49a3* next = node->next;
        if (UnkRefObject* p = node->ref) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
        list->allocator->vf_0x48(node);
        node = next;
    }
    list->count = 0;
    list->head = nullptr;
    list->tail = nullptr;
    fn_71003606c0(self);
}

void fn_710066a370(UnkRodotukSuckAttack_b49a3* self) {
    Destroy_b49a3(self);
}

void fn_710066a4b0(UnkRodotukSuckAttack_b49a3* self) {
    Destroy_b49a3(self);
    operator delete(self);
}
