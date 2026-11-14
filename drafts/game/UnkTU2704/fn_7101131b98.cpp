// CSpawnPointComponent unit (tools/tu_map.py, unit 2704): appends a copy of a lazily resolved holder (0x40
// bytes: vtable, callback vector, state, refcounted handle) to a base::UnkVector of them, growing the array
// by moving the entries (fn_710111fd80). Names and layouts are placeholders.
#include <new>

#include "base/UnkErasedVector.h"
#include "base/UnkRefHandle.h"
#include "base/UnkVector.h"
#include "game/UnkTU341.h"
#include "types.h"

extern void* g_unk_7101cb2880[];  // the holder's vtable (reached through the GOT)

// Moves the holder at the second pointer into the first.
void fn_710111fd80(void*, void*);

struct UnkHolderBase_l131b98 {
    char* vtable;
    UnkOwnedErasedVector callbacks;  // 0x8
    u32 state;                       // 0x30

    UnkHolderBase_l131b98() : vtable(reinterpret_cast<char*>(g_unk_7101a20998) + 0x10), state(0) {}
};

struct UnkHolder_l131b98 : UnkHolderBase_l131b98 {
    base::UnkRefHandle handle;  // 0x38

    UnkHolder_l131b98(const UnkHolder_l131b98& other) {
        vtable = reinterpret_cast<char*>(g_unk_7101cb2880) + 0x10;
        handle.ptr = nullptr;
        handle.Set(other.handle.ptr);
        state = other.state;
        fn_710023b29c(&callbacks, &other.callbacks);
    }
    UnkHolder_l131b98(UnkHolder_l131b98&& other) { fn_710111fd80(this, &other); }

    // The virtual destructor, through the vtable's first slot.
    void Destroy() { (*reinterpret_cast<void (**)(UnkHolder_l131b98*)>(vtable))(this); }
};

void fn_7101131b98(base::UnkVector<UnkHolder_l131b98>* self, const UnkHolder_l131b98* value) {
    u32 n = self->count;
    u32 old_capacity = self->capacity;
    u32 new_capacity = old_capacity;
    if (old_capacity <= n && self->step != 0)
        new_capacity = (n / self->step + 1) * self->step;
    if ((s32)n >= (s32)new_capacity)
        for (;;) {
        }
    UnkHolder_l131b98* p;
    if (old_capacity < new_capacity) {
        p = static_cast<UnkHolder_l131b98*>(self->allocator->vf_0x40((u64)new_capacity * sizeof(UnkHolder_l131b98), 8, self->flags));
        if (UnkHolder_l131b98* src = self->data) {
            UnkHolder_l131b98* dst = p;
            for (u32 i = 0; i < self->count; ++i) {
                fn_710111fd80(dst, src);
                src->Destroy();
                ++src;
                ++dst;
            }
            self->allocator->vf_0x48(self->data);
        }
        n = self->count;
        self->data = p;
        self->capacity = new_capacity;
    } else {
        p = self->data;
    }
    new (&p[n]) UnkHolder_l131b98(*value);
    ++self->count;
}
