#include <new>

#include "base/UnkVector.h"
#include "base/UnkErasedVector.h"
#include "types.h"

struct UnkRefCounted {
    virtual void vf0();
    virtual void vf8();
    virtual void Delete();
    union {
        s32 ref_count;
    };
};

struct UnkCallbackBase {
    virtual void vf0();
    UnkOwnedErasedVector callbacks;
    u32 field_0x30;

    UnkCallbackBase() : field_0x30(0) {}
};

struct UnkCallbackSet_7101a20950 : UnkCallbackBase {
    virtual void vf8();
    UnkRefCounted* owner;

    UnkCallbackSet_7101a20950(const UnkCallbackSet_7101a20950& other) : owner(nullptr) {
        UnkRefCounted* p = other.owner;
        if (p != owner) {
            if (p)
                ++p->ref_count;
            if (UnkRefCounted* old = owner) {
                if (--old->ref_count == 0)
                    old->Delete();
            }
            owner = p;
        }
        field_0x30 = other.field_0x30;
        // The destination's address is taken from raw bytes, before the source's.
        UnkErasedVector* to = reinterpret_cast<UnkErasedVector*>(reinterpret_cast<u8*>(this) + 8);
        fn_710023b29c(to, &other.callbacks);
    }
};

// The move constructor, typed as CActor_VectorCopy_b41.cpp types it (one declaration per symbol).
struct UnkHalf_cu {
    u8 data[0x40];
    UnkHalf_cu(UnkHalf_cu&& other) asm("fn_71002458a4");
};

struct UnkEntry_b41a1 {
    virtual ~UnkEntry_b41a1();
    u8 pad_0x8[0x38];
};

// Appends a copy of the set: the array grows first (its sets moved over and destroyed), and the
// copy is made in place.
void fn_7100246ba8(base::UnkVector<UnkEntry_b41a1>* self, const UnkEntry_b41a1* value) {
    u32 n = self->count;
    u32 capacity = self->capacity;
    u32 new_capacity = capacity;
    if (capacity <= n && self->step != 0)
        new_capacity = (n / self->step + 1) * self->step;
    if ((s32)n >= (s32)new_capacity)  // hangs, as base::UnkVector::PushBack does
        for (;;) {
        }
    UnkEntry_b41a1* p;
    if (capacity < new_capacity) {
        p = static_cast<UnkEntry_b41a1*>(
            self->allocator->vf_0x40((u64)new_capacity * sizeof(UnkEntry_b41a1), 8, self->flags));
        if (UnkEntry_b41a1* src = self->data) {
            UnkEntry_b41a1* dst = p;
            for (u32 i = 0; i < self->count; ++i) {
                new (dst) UnkHalf_cu(static_cast<UnkHalf_cu&&>(*reinterpret_cast<UnkHalf_cu*>(src)));
                (src++)->~UnkEntry_b41a1();
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
    new (&p[n]) UnkCallbackSet_7101a20950(*reinterpret_cast<const UnkCallbackSet_7101a20950*>(value));
    ++self->count;
}
