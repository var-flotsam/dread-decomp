// CSpawnGroupComponent unit (tools/tu_map.py unit 2703): the holder at an index of a holder array, growing
// the array up to it with empty holders when it is past the end (CRntVector::GrowAt written out, the moves
// through fn_710111fd80). Names and layouts are placeholders.
#include <new>

#include "base/UnkErasedVector.h"
#include "base/UnkRefHandle.h"
#include "base/global/CRntVector.h"
#include "game/CSpawnGroupComponent.h"
#include "game/UnkSpawnHolder.h"
#include "game/UnkTU2704.h"
#include "game/UnkTU341.h"
#include "types.h"

// An empty holder as it is built: the callback set's base part first, then the holder's own.
struct UnkSpawnHolderBase_l11a968 {
    void** vtable;
    UnkOwnedErasedVector callbacks;  // 0x8
    u32 state;                       // 0x30

    UnkSpawnHolderBase_l11a968() : vtable(&g_unk_7101a20998[2]), state(0) {}
};

struct UnkSpawnHolderEmpty_l11a968 : UnkSpawnHolderBase_l11a968 {
    base::UnkRefHandle handle;  // 0x38

    UnkSpawnHolderEmpty_l11a968() {
        vtable = &g_unk_7101cb2880[2];
        handle.ptr = nullptr;
    }
};
static_assert(sizeof(UnkSpawnHolderEmpty_l11a968) == sizeof(UnkSpawnHolder), "UnkSpawnHolderEmpty_l11a968 layout");

// Moves the holders to a new array of `capacity` and frees the old one.
static inline void Reallocate(base::global::CRntVector<UnkSpawnHolder>* self, u32 capacity) {
    UnkSpawnHolder* fresh = static_cast<UnkSpawnHolder*>(
        self->allocator->vf_0x40((u64)capacity * sizeof(UnkSpawnHolder), 8, self->flags));
    if (UnkSpawnHolder* src = self->data) {
        UnkSpawnHolder* dst = fresh;
        for (u32 i = 0; i < self->count; ++i) {
            fn_710111fd80(dst, src);
            (src++)->~UnkSpawnHolder();
            ++dst;
        }
        self->allocator->vf_0x48(self->data);
    }
    self->data = fresh;
    self->capacity = capacity;
}

UnkSpawnHolder* fn_710111a968(base::global::CRntVector<UnkSpawnHolder>* self, u32 index) {
    if (self->field_0x14 && self->count <= index) {
        // First to the next multiple of the step, then to exactly index + 1 holders.
        u32 cap = self->capacity;
        if (cap <= index && self->step != 0) {
            u32 grown = (index / self->step + 1) * self->step;
            if (cap < grown) {
                Reallocate(self, grown);
                cap = grown;
            }
        }
        u32 n = index + 1;
        if (cap < n)
            Reallocate(self, n);
        for (u32 i = self->count; i < n; ++i)
            new (&self->data[i]) UnkSpawnHolderEmpty_l11a968();
        self->count = n;
    }
    return &self->data[index];
}
