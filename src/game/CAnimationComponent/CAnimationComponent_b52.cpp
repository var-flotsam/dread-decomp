// CAnimationComponent unit (tools/tu_map.py, the name from reflection). Names and layouts are placeholders.
#include "base/UnkCountedResource.h"
#include "base/UnkVector.h"
#include "types.h"

struct UnkOwner_710070f240;  // src/game_lo/fn_710070f240.cpp
void fn_710070f240(UnkOwner_710070f240* self, u32 value);

struct UnkAnimComponent_b52 {
    u8 pad_0x0[0x4c0];
    u8 list_0x4c0[0x28];  // UnkOwner_710070f240
};

void fn_7101178c88(UnkAnimComponent_b52* self, bool flag, f32 value);

// Removes `value` from the list at +0x4c0 and, when `apply` is set, runs fn_7101178c88.
void fn_7101178e08(UnkAnimComponent_b52* self, u32 value, bool apply, bool flag, f32 time) {
    fn_710070f240(reinterpret_cast<UnkOwner_710070f240*>(self->list_0x4c0), value);
    if (apply)
        fn_7101178c88(self, flag, time);
}

// A counted reference whose move constructor starts empty and assigns: it takes a new reference to the other's
// resource (the other keeps its own until it is destroyed).
struct UnkAnimResRef_b52 {
    UnkCountedResource* ptr;

    UnkAnimResRef_b52(UnkAnimResRef_b52&& other) {
        ptr = nullptr;
        if (ptr != other.ptr) {
            UnkCountedResource* p = other.ptr;
            if (p)
                ++p->ref_count;
            UnkCountedResource* old = ptr;
            if (old && --old->ref_count == 0)
                fn_710001737c(old);
            ptr = p;
        }
    }
    ~UnkAnimResRef_b52() {
        UnkCountedResource* p = ptr;
        if (p && --p->ref_count == 0)
            fn_710001737c(p);
    }
};

// Grows the array of resource references to `capacity` elements (base::UnkVector's Reserve, written
// out: the element type's move and destroy).
void fn_710117c2e4(base::UnkVector<UnkAnimResRef_b52>* self, u32 capacity) {
    if (self->capacity < capacity) {
        auto* p = static_cast<UnkAnimResRef_b52*>(
            self->allocator->vf_0x40((u64)capacity * sizeof(UnkAnimResRef_b52), 8, self->flags));
        if (self->data) {
            UnkAnimResRef_b52* src = self->data;
            UnkAnimResRef_b52* dst = p;
            for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
                new (dst) UnkAnimResRef_b52(static_cast<UnkAnimResRef_b52&&>(*src));
                src->~UnkAnimResRef_b52();
            }
            self->allocator->vf_0x48(self->data);
        }
        self->data = p;
        self->capacity = capacity;
    }
}

// Three pointer lists and a lock count (+0x78).
struct UnkAnimListeners_b52 {
    base::UnkVector<void*> active;   // 0x0
    base::UnkVector<void*> pending;  // 0x28
    base::UnkVector<void*> queued;   // 0x50
    s32 lock;                        // 0x78
};

// Removes the first `value` from the array (data, count), shifting the rest down.
static inline __attribute__((always_inline)) void RemoveFirst(base::UnkVector<void*>& v, void* value) {
    u32 count = v.count;
    if (count == 0)
        return;
    void** data = v.data;
    for (u32 i = 0; i < count; ++i) {
        if (data[i] == value) {
            __builtin_memcpy(&data[i], &data[i + 1], (u64)(count - i - 1) * sizeof(void*));
            --v.count;
            return;
        }
    }
}

// While locked, appends `value` to the pending list and takes it out of the active one; otherwise
// takes it out of the queued list.
void fn_710117eb1c(UnkAnimListeners_b52* self, void* value) {
    if (self->lock <= 0) {
        RemoveFirst(self->queued, value);
    } else {
        self->pending.PushBack(value);
        RemoveFirst(self->active, value);
    }
}
