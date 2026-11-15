// CGameManager::CTunableGameManager's unit (tools/tu_map.py unit 2553): registers an object under an id,
// appending its weak handle (created on first use) to the id's list. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "game/UnkGameManager.h"
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

void fn_7100de9540(UnkGameManager* manager, const u64* id, UnkWeakTarget* object) {
    UnkGameManagerIdList* list = manager->field_0x2450.Find(*id);
    UnkHandleRef_de9540 handle(GetWeakHandle(object));
    reinterpret_cast<UnkHandleList_de9540*>(list)->PushBack(handle);
}
