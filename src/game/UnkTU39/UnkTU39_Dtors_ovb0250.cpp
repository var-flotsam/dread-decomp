// Unit 39 of tools/tu_map.py (no reliable name): destructors of the keyed list (vtable, count, head,
// tail, allocator, a deferred callback set at +0x30; nodes of {string id, float, float, next}). The set's
// callbacks forget their owner, the three arrays are freed, then the list releases every id and node.
// Two copies with their own vtables. Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkDeferredSet.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"

using base::UnkDeferredCallback;
using base::UnkDeferredSet;
using base::global::CStrId;

extern void* g_unk_7101a1a090[];
extern void* g_unk_7101a1a0b0[];

struct UnkKeyNode_b0250 {
    CStrId id;
    f32 value;
    f32 value2;
    UnkKeyNode_b0250* next;
};

template <void** Vtable>
struct UnkKeyListBase_b0250 {
    void** vtable;
    u32 count;
    UnkKeyNode_b0250* head;
    UnkKeyNode_b0250* tail;
    base::UnkAllocator* allocator;
    u32 tag;

    ~UnkKeyListBase_b0250() {
        vtable = &Vtable[2];
        for (UnkKeyNode_b0250* n = head; n;) {
            UnkKeyNode_b0250* next = n->next;
            n->id.Reset();
            allocator->vf_0x48(n);
            n = next;
        }
        count = 0;
        head = nullptr;
        tail = nullptr;
    }
};

template <void** Vtable>
struct UnkKeyList_b0250 : UnkKeyListBase_b0250<Vtable> {
    UnkDeferredSet<UnkDeferredCallback<f32>> on_change;  // 0x30

    ~UnkKeyList_b0250() {
        for (UnkDeferredCallback<f32>**it = on_change.items.data, **end = it + on_change.items.count; it != end; ++it)
            (*it)->field_0x0 = nullptr;
        on_change.items.count = 0;
        on_change.pending_add.count = 0;
        on_change.pending_remove.count = 0;
        on_change.pending_remove.Free();
        on_change.pending_add.Free();
        on_change.items.Free();
    }
};

struct UnkMoveSlot_0xb0;

__attribute__((flatten)) void fn_710002f67c(void* self) {
    static_cast<UnkKeyList_b0250<g_unk_7101a1a090>*>(self)->~UnkKeyList_b0250();
}

__attribute__((flatten)) void fn_7100030a50(UnkMoveSlot_0xb0* self) {
    reinterpret_cast<UnkKeyList_b0250<g_unk_7101a1a0b0>*>(self)->~UnkKeyList_b0250();
}
