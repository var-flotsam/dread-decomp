// sound::CSoundManager unit (tools/tu_map.py, unit 2646): adds a sound handle to the list kept under
// its sound's key (creating the list when the key has none), unless the list already holds a handle
// on that sound. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CSwarmControllerComponent.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::global::CStrId;

// A weak reference, as src/game/CShotManager.cpp has it.
struct UnkShotWeakTarget {
    void* field_0x0;
    s64 generation;
};
struct UnkShotWeakRef {
    UnkShotWeakTarget* target;
    s64 generation;
};

// What a live reference points at, as far as its key.
struct UnkRefObject_l0244a8 {
    u8 pad_0x0[0x88];
    CStrId key;  // 0x88
};

inline void* Resolve(UnkShotWeakRef ref) {
    UnkShotWeakTarget* target = ref.target;
    if (target && ref.generation != -1 && target->generation == ref.generation)
        return target->field_0x0;
    return nullptr;
}

using UnkWeakRefList_l0244a8 = base::global::CRntVector<UnkShotWeakRef>;

template <class V>
struct UnkIdMap;
struct UnkOwnedList;
void fn_710102c0e8(UnkIdMap<UnkOwnedList>* self, u32 capacity);

// From key to the list of references: two parallel arrays sharing one count.
struct UnkWeakRefIndex_l0244a8 {
    UnkWeakRefList_l0244a8* lists;  // 0x0
    CStrId* keys;                   // 0x8
    u32 count;                      // 0x10
    u32 capacity;                   // 0x14
    u32 step;                       // 0x18

    UnkWeakRefList_l0244a8* End() { return &lists[count]; }
    UnkWeakRefList_l0244a8* Find(const CStrId& key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i].data == key.data)
                return &lists[i];
        return End();
    }
    void Add(const CStrId& key, UnkWeakRefList_l0244a8&& list) {
        if (count == capacity)
            fn_710102c0e8(reinterpret_cast<UnkIdMap<UnkOwnedList>*>(this), count + step);
        new (&keys[count]) CStrId(key);
        new (&lists[count]) UnkWeakRefList_l0244a8(static_cast<UnkWeakRefList_l0244a8&&>(list));
        ++count;
    }
};

struct UnkDropTarget;

struct UnkSoundManager_l0244a8 {
    u8 pad_0x0[0x68];
    UnkWeakRefIndex_l0244a8 index;  // 0x68
};

// Appends a reference, growing the array by its step (the elements copied one by one).
static inline void Append(UnkWeakRefList_l0244a8* list, const UnkShotWeakRef& ref) {
    u32 n = list->count;
    u32 old_capacity = list->capacity;
    u32 new_capacity = old_capacity;
    if (old_capacity <= n && list->step != 0)
        new_capacity = (n / list->step + 1) * list->step;
    // The original hangs here when the array cannot grow.
    if ((s32)n >= (s32)new_capacity)
        for (;;) {
        }
    if (old_capacity < new_capacity) {
        UnkShotWeakRef* p = static_cast<UnkShotWeakRef*>(
            list->allocator->vf_0x40((u64)new_capacity * sizeof(UnkShotWeakRef), 8, list->flags));
        if (list->data) {
            UnkShotWeakRef* src = list->data;
            UnkShotWeakRef* dst = p;
            for (u32 i = 0; i < list->count; ++i, ++dst, ++src)
                *dst = *src;
            list->allocator->vf_0x48(list->data);
        }
        list->data = p;
        list->capacity = new_capacity;
        n = list->count;
    }
    list->data[n] = ref;
    ++list->count;
}

void fn_71010244a8(UnkDropTarget* target, UnkSoundHandle handle) {
    UnkSoundManager_l0244a8* self = reinterpret_cast<UnkSoundManager_l0244a8*>(target);
    UnkShotWeakRef ref = {reinterpret_cast<UnkShotWeakTarget*>(handle.ref), handle.serial};
    void* object = Resolve(ref);
    if (!object)
        return;
    const CStrId& key = static_cast<UnkRefObject_l0244a8*>(object)->key;
    if (self->index.Find(key) == self->index.End())
        self->index.Add(key, UnkWeakRefList_l0244a8(0xf));
    UnkWeakRefList_l0244a8* list = self->index.Find(key);
    bool present = false;
    for (s32 i = list->count; !present && i-- > 0;) {
        UnkShotWeakRef entry = *fn_7100748064(list, i);
        present |= Resolve(ref) == Resolve(entry);
    }
    if (present)
        return;
    Append(list, ref);
}
