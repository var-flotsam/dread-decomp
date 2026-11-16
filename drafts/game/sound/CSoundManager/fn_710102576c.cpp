// sound::CSoundManager unit (tools/tu_map.py, unit 2646): removes a weak reference from the list kept
// under its target's key, the last matching entry only. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/CSwarmControllerComponent.h"
#include "types.h"

// A weak reference, as src/game/CShotManager.cpp has it.
struct UnkShotWeakTarget {
    void* field_0x0;
    s64 generation;
};
struct UnkShotWeakRef {
    UnkShotWeakTarget* target;
    s64 generation;
};

// What the weak reference's target points at, as far as its key.
struct UnkRefObject_l02576c {
    u8 pad_0x0[0x88];
    u64 key;  // 0x88
};

inline void* Resolve(const UnkShotWeakRef& ref) {
    UnkShotWeakTarget* target = ref.target;
    if (target && ref.generation != -1 && target->generation == ref.generation)
        return target->field_0x0;
    return nullptr;
}

struct UnkSoundManager_l02576c {
    u8 pad_0x0[0x68];
    base::global::CRntVector<UnkShotWeakRef>* lists;  // 0x68
    u64* keys;                                        // 0x70
    u32 list_count;                                   // 0x78

    base::global::CRntVector<UnkShotWeakRef>* End() { return &lists[list_count]; }
    base::global::CRntVector<UnkShotWeakRef>* Find(const u64& key) {
        for (u64 i = 0; i < list_count; ++i)
            if (keys[i] == key)
                return &lists[i];
        return End();
    }
};

void fn_710102576c(UnkSoundManager_l02576c* self, UnkShotWeakRef ref) {
    void* object = Resolve(ref);
    if (!object)
        return;
    const u64& key = static_cast<UnkRefObject_l02576c*>(object)->key;
    if (self->Find(key) == self->End())
        return;
    base::global::CRntVector<UnkShotWeakRef>* list = self->Find(key);
    s32 i = list->count;
    while (i-- > 0) {
        UnkShotWeakRef* entry = fn_7100748064(list, i);
        if (Resolve(ref) == Resolve(*entry)) {
            if (list->count > static_cast<u32>(i)) {
                for (u32 j = i; j < list->count - 1; ++j)
                    list->data[j] = list->data[j + 1];
                --list->count;
            }
            return;
        }
    }
}
