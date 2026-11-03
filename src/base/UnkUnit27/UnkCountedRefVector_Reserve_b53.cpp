// Unit 27 (tools/tu_map.py): grows an array of counted resource references to `capacity` elements
// (base::UnkVector's Reserve written out, as fn_710117c2e4 in
// src/game/CAnimationComponent/CAnimationComponent_b52.cpp). The signature is the one
// src/engine/UnkGraphicsSystem.cpp declares. Names and layouts are placeholders.
#include "base/UnkCountedResource.h"
#include "base/UnkVector.h"
#include "types.h"

// A counted reference whose move constructor starts empty and takes a new reference to the other's
// resource (the other keeps its own until it is destroyed).
struct UnkCountedRef_b53 {
    UnkCountedResource* ptr;

    UnkCountedRef_b53(UnkCountedRef_b53&& other) {
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
    ~UnkCountedRef_b53() {
        UnkCountedResource* p = ptr;
        if (p && --p->ref_count == 0)
            fn_710001737c(p);
    }
};

void fn_71000231f4(base::UnkVector<u8>* vector, u32 capacity) {
    auto* self = reinterpret_cast<base::UnkVector<UnkCountedRef_b53>*>(vector);
    if (self->capacity < capacity) {
        auto* p = static_cast<UnkCountedRef_b53*>(
            self->allocator->vf_0x40((u64)capacity * sizeof(UnkCountedRef_b53), 8, self->flags));
        if (self->data) {
            UnkCountedRef_b53* src = self->data;
            UnkCountedRef_b53* dst = p;
            for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
                new (dst) UnkCountedRef_b53(static_cast<UnkCountedRef_b53&&>(*src));
                src->~UnkCountedRef_b53();
            }
            self->allocator->vf_0x48(self->data);
        }
        self->data = p;
        self->capacity = capacity;
    }
}
