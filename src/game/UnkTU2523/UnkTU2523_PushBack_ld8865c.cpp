// UnkTU2523 (tools/tu_map.py unit 2523): appends a copy of a callback set (0x40 bytes: vtable, callback
// vector, state, refcounted owner) to a base::global::CRntVector of them, growing the array by moving the
// entries (fn_71007b1928). Names and layouts are placeholders.
#include <new>

#include "base/UnkErasedVector.h"
#include "base/UnkRefHandle.h"
#include "base/global/CRntVector.h"
#include "game/CCentralUnitAIComponent.h"
#include "game/CDemolitionBlockComponent.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU341.h"
#include "types.h"

struct UnkOwnedSet_ld8865c {
    virtual ~UnkOwnedSet_ld8865c();
    UnkOwnedErasedVector callbacks;  // 0x8
    u32 state;                       // 0x30
    base::UnkRefHandle owner;        // 0x38

    // Makes this (just constructed) a copy of `other`.
    void CopyFrom(const UnkOwnedSet_ld8865c& other) {
        state = 0;
        *reinterpret_cast<char**>(this) = reinterpret_cast<char*>(g_unk_7101a33fa0) + 0x10;
        owner.ptr = nullptr;
        if (base::UnkRefCounted* p = other.owner.ptr) {
            ++p->ref_count;
            if (base::UnkRefCounted* old = owner.ptr) {
                if (--old->ref_count == 0)
                    delete old;
            }
            owner.ptr = p;
        }
        state = other.state;
        fn_710023b29c(&callbacks, &other.callbacks);
    }
};
static_assert(sizeof(UnkOwnedSet_ld8865c) == 0x40, "layout");

// The unit header declares the entry as UnkEntry_b43a3 with no layout; this file reads it as a callback set.
void fn_7100d8865c(base::global::CRntVector<UnkEntry_b43a3>* vector, const UnkEntry_b43a3* entry) {
    auto* self = reinterpret_cast<base::global::CRntVector<UnkOwnedSet_ld8865c>*>(vector);
    auto* value = reinterpret_cast<const UnkOwnedSet_ld8865c*>(entry);
    u32 n = self->count;
    u32 old_capacity = self->capacity;
    u32 new_capacity = old_capacity;
    if (old_capacity <= n && self->step != 0)
        new_capacity = (n / self->step + 1) * self->step;
    if ((s32)n >= (s32)new_capacity)
        for (;;) {
        }
    UnkOwnedSet_ld8865c* p;
    if (old_capacity < new_capacity) {
        p = static_cast<UnkOwnedSet_ld8865c*>(
            self->allocator->vf_0x40((u64)new_capacity * sizeof(UnkOwnedSet_ld8865c), 8, self->flags));
        if (UnkOwnedSet_ld8865c* src = self->data) {
            UnkOwnedSet_ld8865c* dst = p;
            for (u32 i = 0; i < self->count; ++i) {
                fn_71007b1928(dst, src);
                (src++)->~UnkOwnedSet_ld8865c();
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
    UnkOwnedSet_ld8865c* e = &p[n];
    new (e) UnkOwnedSet_ld8865c;
    e->CopyFrom(*value);
    ++self->count;
}
