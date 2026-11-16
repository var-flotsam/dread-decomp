// CGameManager::CTunableGameManager's unit (tools/tu_map.py unit 2553): unregisters an object from an id,
// removing the first entry of the id's list that leads to the same object as its weak handle (created on
// first use). Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

// Signature as src/game/CActor/UnkRefPtrSlotReserve_b42.cpp defines it.
template <class Tag>
struct UnkSlotArray;
struct UnkTag_7100336b54;
void fn_7100336b54(UnkSlotArray<UnkTag_7100336b54>* self, u32 capacity);

// A counted reference to a weak handle, held while it is appended.
struct UnkHandleRef_de9540 {
    UnkWeakHandle* handle;

    explicit UnkHandleRef_de9540(UnkWeakHandle* p) : handle(p) {
        if (handle)
            ++handle->ref_count;
    }
    ~UnkHandleRef_de9540() {
        if (handle && --handle->ref_count == 0)
            handle->vf_0x10();
    }
};

// A slot of the list: a counted reference to a weak handle.
struct UnkHandleSlot_de9540 {
    UnkWeakHandle* handle;

    void Assign(UnkWeakHandle* p) {
        if (p) {
            ++p->ref_count;
            if (UnkWeakHandle* old = handle) {
                if (--old->ref_count == 0)
                    old->vf_0x10();
            }
            handle = p;
        }
    }
};

// One id's list of weak handles (base::global::CRntVector's layout, its reserve out of line).
struct UnkHandleList_de9540 {
    UnkHandleSlot_de9540* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool growable;
    u32 flags;
    base::UnkAllocator* allocator;

    void PushBack(const UnkHandleRef_de9540& value) {
        u32 n = count;
        u32 new_capacity = capacity;
        if (new_capacity <= n && step != 0)
            new_capacity = (n / step + 1) * step;
        // A capacity that doesn't grow past the count (no step) hangs, as in the original.
        if ((s32)n >= (s32)new_capacity)
            for (;;) {
            }
        fn_7100336b54(reinterpret_cast<UnkSlotArray<UnkTag_7100336b54>*>(this), new_capacity);
        UnkHandleSlot_de9540* slot = new (&data[count]) UnkHandleSlot_de9540();
        slot->Assign(value.handle);
        ++count;
    }
};

// What a weak handle leads to (nothing for no handle).
static inline UnkWeakTarget* TargetOf_de9730(UnkWeakHandle* handle) {
    return handle ? handle->target : nullptr;
}

// Drops a slot's reference without clearing it.
static inline void Release_de9730(UnkHandleSlot_de9540& slot) {
    if (UnkWeakHandle* handle = slot.handle) {
        if (--handle->ref_count == 0)
            handle->vf_0x10();
    }
}

// The index of the first slot that leads to what `handle` leads to, or the count when there is none.
static inline u32 IndexOf_de9730(UnkHandleList_de9540* list, UnkWeakHandle* handle) {
    u32 n = list->count;
    for (u32 i = 0; i < n; ++i) {
        if (TargetOf_de9730(list->data[i].handle) == TargetOf_de9730(handle))
            return i;
    }
    return n;
}

// Removes the slot at `index`, moving the later ones down.
static inline void EraseAt_de9730(UnkHandleList_de9540* list, u32 index) {
    if (index < list->count) {
        Release_de9730(list->data[index]);
        for (u32 i = index; i < list->count - 1; ++i) {
            UnkWeakHandle* next = list->data[i + 1].handle;
            new (&list->data[i]) UnkHandleSlot_de9540();
            list->data[i].Assign(next);
            Release_de9730(list->data[i + 1]);
        }
        list->count = list->count - 1;
    }
}

void fn_7100de9730(UnkGameManager* manager, UnkSpawnGroupKey* key, void* object) {
    UnkGameManagerIdList* ids = manager->field_0x2450.Find(*reinterpret_cast<const u64*>(key));
    UnkHandleRef_de9540 handle(GetWeakHandle(static_cast<UnkWeakTarget*>(object)));
    auto* list = reinterpret_cast<UnkHandleList_de9540*>(ids);
    EraseAt_de9730(list, IndexOf_de9730(list, handle.handle));
}
