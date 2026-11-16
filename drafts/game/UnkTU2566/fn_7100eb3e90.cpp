// CMinimapManager::CTunableMinimapManager's unit (tools/tu_map.py): grows two parallel arrays (counted resource
// references and their string ids) that share a count, a capacity and allocation flags to `capacity` elements;
// the references are moved over and released, the ids taken over. Names and layouts are placeholders.
#include <new>

#include "base/UnkCountedResource.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::UnkAllocator;
using base::global::CStrId;

// As src/base/UnkUnit27/UnkCountedRefVector_Reserve_b53.cpp defines it.
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

struct UnkNamedRefArrays_eb3e90 {
    UnkCountedRef_b53* refs;
    CStrId* names;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    UnkAllocator* allocator;
};

void fn_7100eb3e90(UnkNamedRefArrays_eb3e90* self, u32 capacity) {
    if (self->capacity < capacity) {
        auto* refs = static_cast<UnkCountedRef_b53*>(
            self->allocator->vf_0x40((u64)capacity * sizeof(UnkCountedRef_b53), 8, self->flags));
        auto* names = static_cast<CStrId*>(self->allocator->vf_0x40((u64)capacity * sizeof(CStrId), 8, self->flags));
        if (UnkCountedRef_b53* old = self->refs) {
            UnkCountedRef_b53* dst = refs;
            for (u32 i = 0; i < self->count; ++i, ++dst, ++old) {
                new (dst) UnkCountedRef_b53(static_cast<UnkCountedRef_b53&&>(*old));
                old->~UnkCountedRef_b53();
            }
            self->allocator->vf_0x48(self->refs);
        }
        if (CStrId* old = self->names) {
            CStrId* dst = names;
            for (u32 i = 0; i < self->count; ++i, ++old, ++dst) {
                dst->data = old->data;
                old->data = &g_unk_7101d47108->empty;
                old->Reset();
            }
            self->allocator->vf_0x48(self->names);
        }
        self->refs = refs;
        self->names = names;
        self->capacity = capacity;
    }
}
