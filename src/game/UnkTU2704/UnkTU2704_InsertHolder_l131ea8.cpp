// CSpawnPointComponent's unit (unit 2704): inserts a copy of a holder (0x40 bytes: vtable, callback vector
// at +0x8, state at +0x30, refcounted handle at +0x38) at an index of the holder array, moving the later
// entries up one place (growing the array first when it is full); appending is fn_7101131b98.
// Names and layouts are placeholders.
#include "base/UnkErasedVector.h"
#include "base/UnkRefHandle.h"
#include "base/UnkVector.h"
#include "game/CSpawnGroupComponent.h"
#include "game/UnkTU2704.h"
#include "types.h"


// As src/clones/clones_06.cpp declares it: moves the entry at the second pointer into the first.

struct UnkSpawnHolder_l131ea8 {
    virtual ~UnkSpawnHolder_l131ea8();
    UnkOwnedErasedVector callbacks;  // 0x8
    u32 state;                       // 0x30
    base::UnkRefHandle handle;       // 0x38

    void CopyFrom(const UnkSpawnHolder_l131ea8& other) {
        state = 0;
        *reinterpret_cast<char**>(this) = reinterpret_cast<char*>(g_unk_7101cb2880) + 0x10;
        handle.ptr = nullptr;
        if (base::UnkRefCounted* p = other.handle.ptr) {
            ++p->ref_count;
            if (base::UnkRefCounted* old = handle.ptr) {
                if (--old->ref_count == 0)
                    delete old;
            }
            handle.ptr = p;
        }
        state = other.state;
        fn_710023b29c(&callbacks, &other.callbacks);
    }
};
static_assert(sizeof(UnkSpawnHolder_l131ea8) == 0x40, "layout");

// The append, as src/game/CSpawnPointComponent/CSpawnPointComponent_b42.cpp declares it (its entry type is this holder).
struct UnkEntry_b42a3;

void fn_7101131ea8(base::UnkVector<UnkSpawnHolder_l131ea8>* self, const UnkSpawnHolder_l131ea8* value, u32 index) {
    u32 n = self->count;
    if (n == index) {
        fn_7101131b98(reinterpret_cast<base::UnkVector<UnkEntry_b42a3>*>(self),
                      reinterpret_cast<const UnkEntry_b42a3*>(value));
        return;
    }
    if (self->capacity <= n && self->step != 0) {
        u32 capacity = (n / self->step + 1) * self->step;
        if (self->capacity < capacity) {
            UnkSpawnHolder_l131ea8* fresh = static_cast<UnkSpawnHolder_l131ea8*>(
                self->allocator->vf_0x40((u64)capacity * sizeof(UnkSpawnHolder_l131ea8), 8, self->flags));
            if (UnkSpawnHolder_l131ea8* src = self->data) {
                UnkSpawnHolder_l131ea8* dst = fresh;
                for (u32 i = 0; i < self->count; ++i) {
                    fn_710111fd80(dst, src);
                    (src++)->~UnkSpawnHolder_l131ea8();
                    ++dst;
                }
                self->allocator->vf_0x48(self->data);
            }
            n = self->count;
            self->data = fresh;
            self->capacity = capacity;
        }
    }
    // The entries from the last down to the index move up one place (the index runs through -1 when it is 0).
    for (u32 i = n - 1; i >= index; --i) {
        UnkSpawnHolder_l131ea8* from = &self->data[i];
        fn_710111fd80(from + 1, from);
        self->data[i].~UnkSpawnHolder_l131ea8();
        if (i == 0)
            break;
    }
    UnkSpawnHolder_l131ea8* e = &self->data[index];
    new (e) UnkSpawnHolder_l131ea8;
    e->CopyFrom(*value);
    ++self->count;
}
