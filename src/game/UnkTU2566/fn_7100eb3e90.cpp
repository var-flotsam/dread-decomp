// CMinimapManager::CTunableMinimapManager's unit (tools/tu_map.py): grows two parallel arrays (counted resource
// references and their string ids) that share a count, a capacity and allocation flags to `capacity` elements;
// the references are moved over and released, the ids taken over. Names and layouts are placeholders.
#include <new>

#include "base/UnkCountedResource.h"
#include "base/global/CRntSmallDictionary.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkCountedRef_b53.h"
#include "types.h"

using base::IAllocator;
using base::global::CStrId;

using UnkNamedRefArrays_eb3e90 = base::global::CRntSmallDictionary<CStrId, UnkCountedRef_b53>;

void fn_7100eb3e90(UnkNamedRefArrays_eb3e90* self, u32 capacity) {
    if (self->capacity < capacity) {
        auto* refs = static_cast<UnkCountedRef_b53*>(
            self->allocator->vf_0x40((u64)capacity * sizeof(UnkCountedRef_b53), 8, self->flags));
        auto* names = static_cast<CStrId*>(self->allocator->vf_0x40((u64)capacity * sizeof(CStrId), 8, self->flags));
        if (UnkCountedRef_b53* old = self->values) {
            UnkCountedRef_b53* dst = refs;
            for (u32 i = 0; i < self->count; ++i, ++dst, ++old) {
                new (dst) UnkCountedRef_b53(static_cast<UnkCountedRef_b53&&>(*old));
                old->~UnkCountedRef_b53();
            }
            self->allocator->vf_0x48(self->values);
        }
        if (CStrId* old = self->keys) {
            CStrId* dst = names;
            for (u32 i = 0; i < self->count; ++i, ++old, ++dst) {
                CStrId::MoveConstructReloading(dst, *old);
                old->Reset();
            }
            self->allocator->vf_0x48(self->keys);
        }
        self->values = refs;
        self->keys = names;
        self->capacity = capacity;
    }
}
