// navmesh::CNavMeshGenerator's unit: a copy of the resource-reference array Reserve in
// src/game/CAnimationComponent/CAnimationComponent_b52.cpp (fn_710117c2e4): grows the array to
// `capacity` elements, moving each counted reference and releasing the old one. Names are placeholders.
#include "base/UnkCountedResource.h"
#include "base/UnkVector.h"
#include "types.h"

// A counted reference whose move constructor starts empty and takes a new reference to the other's resource.
struct UnkNavResRef_b64a2 {
    UnkCountedResource* ptr;

    UnkNavResRef_b64a2(UnkNavResRef_b64a2&& other) {
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
    ~UnkNavResRef_b64a2() {
        UnkCountedResource* p = ptr;
        if (p && --p->ref_count == 0)
            fn_710001737c(p);
    }
};

void fn_71002a790c(base::UnkVector<UnkNavResRef_b64a2>* self, u32 capacity) {
    if (self->capacity < capacity) {
        auto* p = static_cast<UnkNavResRef_b64a2*>(
            self->allocator->vf_0x40((u64)capacity * sizeof(UnkNavResRef_b64a2), 8, self->flags));
        if (self->data) {
            UnkNavResRef_b64a2* src = self->data;
            UnkNavResRef_b64a2* dst = p;
            for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
                new (dst) UnkNavResRef_b64a2(static_cast<UnkNavResRef_b64a2&&>(*src));
                src->~UnkNavResRef_b64a2();
            }
            self->allocator->vf_0x48(self->data);
        }
        self->data = p;
        self->capacity = capacity;
    }
}
