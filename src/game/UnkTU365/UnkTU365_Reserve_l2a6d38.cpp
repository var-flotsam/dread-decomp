// Unit 365 (tools/tu_map.py; navmesh::CNavMeshGenerator's neighbours): two parallel arrays (counted
// resource references, and a string id per reference) sharing a count, a capacity and allocation
// flags. Grows both to a new capacity. Names and layouts are placeholders.
#include <new>

#include "base/UnkCountedResource.h"
#include "base/global/CRntSmallDictionary.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

// A counted reference whose move constructor starts empty and takes a new reference to the other's
// resource (the other keeps its own until it is destroyed).
struct UnkCountedRef_l2a6d38 {
    UnkCountedResource* ptr;

    UnkCountedRef_l2a6d38(UnkCountedRef_l2a6d38&& other) {
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
    ~UnkCountedRef_l2a6d38() {
        UnkCountedResource* p = ptr;
        if (p && --p->ref_count == 0)
            fn_710001737c(p);
    }
};

using UnkCountedRefArrays_l2a6d38 = base::global::CRntSmallDictionary<CStrId, UnkCountedRef_l2a6d38>;

// Grows both arrays to `new_capacity`: the references are moved and the old ones dropped, the
// names moved.
void fn_71002a6d38(UnkCountedRefArrays_l2a6d38* self, u32 new_capacity) {
    if (self->capacity >= new_capacity)
        return;
    UnkCountedRef_l2a6d38* refs = static_cast<UnkCountedRef_l2a6d38*>(
        self->allocator->vf_0x40((u64)new_capacity * sizeof(UnkCountedRef_l2a6d38), 8, self->flags));
    CStrId* names = static_cast<CStrId*>(self->allocator->vf_0x40((u64)new_capacity * sizeof(CStrId), 8, self->flags));
    if (UnkCountedRef_l2a6d38* old = self->values) {
        u64 j = 0;
        for (u32 i = 0; i < self->count; ++i, ++j) {
            new (&refs[j]) UnkCountedRef_l2a6d38(static_cast<UnkCountedRef_l2a6d38&&>(old[j]));
            old[j].~UnkCountedRef_l2a6d38();
        }
        self->allocator->vf_0x48(self->values);
    }
    if (CStrId* old = self->keys) {
        u64 j = 0;
        for (u32 i = 0; i < self->count; ++i, ++j, ++old) {
            CStrId::MoveConstructReloading(&names[j], *old);
            old->Reset();
        }
        self->allocator->vf_0x48(self->keys);
    }
    self->values = refs;
    self->keys = names;
    self->capacity = new_capacity;
}
