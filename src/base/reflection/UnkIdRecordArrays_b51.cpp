// Two parallel arrays (0x18-byte records that start with an id, and a CStrId per record) that
// share a count, a capacity and allocation flags; the same shape as the tunables' registry
// (fn_710013ec80, src/base/UnkTunableRegistry.cpp). Names and layouts are placeholders.
#include "base/global/CRntSmallDictionary.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::UnkAllocator;
using base::global::CStrId;


struct UnkIdRecord18B {
    CStrId id;
    u64 field_0x8;
    u32 field_0x10;
    u16 field_0x14;
};
static_assert(sizeof(UnkIdRecord18B) == 0x18, "");


using UnkIdRecord18BArrays = base::global::CRntSmallDictionary<CStrId, UnkIdRecord18B>;

// Grows both arrays to `new_capacity`: the records are copied and their ids released, the
// names moved.
void fn_71000a6ca8(UnkIdRecord18BArrays* self, u32 new_capacity) {
    if (self->capacity < new_capacity) {
        UnkIdRecord18B* records = static_cast<UnkIdRecord18B*>(
            self->allocator->vf_0x40((u64)new_capacity * sizeof(UnkIdRecord18B), 8, self->flags));
        CStrId* names = static_cast<CStrId*>(self->allocator->vf_0x40((u64)new_capacity * sizeof(CStrId), 8, self->flags));
        if (UnkIdRecord18B* old = self->values) {
            UnkIdRecord18B* dst = records;
            for (u32 i = 0; i < self->count; ++i, ++dst, ++old) {
                new (&dst->id) CStrId(old->id);
                dst->field_0x8 = old->field_0x8;
                dst->field_0x10 = old->field_0x10;
                dst->field_0x14 = old->field_0x14;
                old->id.Reset();
            }
            self->allocator->vf_0x48(self->values);
        }
        if (CStrId* old = self->keys) {
            CStrId* dst = names;
            for (u32 i = 0; i < self->count; ++i, ++old, ++dst) {
                union Slot {
                    base::global::UnkStringData* p;
                };
                dst->data = old->data;
                reinterpret_cast<Slot*>(&old->data)->p = &g_unk_7101d47108->empty;
                old->Reset();
            }
            self->allocator->vf_0x48(self->keys);
        }
        self->values = records;
        self->keys = names;
        self->capacity = new_capacity;
    }
}
