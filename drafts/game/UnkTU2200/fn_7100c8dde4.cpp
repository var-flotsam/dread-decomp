// CCutSceneDef's unit (2200): shrinks two parallel arrays (records holding two string ids, and a string id
// per record) to their count: the records are moved and destroyed, the ids copied and released. Names and
// layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkIdPairRecord_lc8dde4 {
    CStrId first;
    CStrId second;
    u32 field_0x10;

    // Takes over the other's ids; the other is left with empty ones.
    void MoveFrom(UnkIdPairRecord_lc8dde4& other) {
        CStrId::MoveConstructReloading(&first, other.first);
        CStrId::MoveConstructReloading(&second, other.second);
        field_0x10 = other.field_0x10;
    }
};
static_assert(sizeof(UnkIdPairRecord_lc8dde4) == 0x18, "");

struct UnkIdPairArrays_lc8dde4 {
    UnkIdPairRecord_lc8dde4* records;
    CStrId* ids;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::IAllocator* allocator;
};

void fn_7100c8dde4(UnkIdPairArrays_lc8dde4* self) {
    if (self->count == self->capacity)
        return;
    UnkIdPairRecord_lc8dde4* records = static_cast<UnkIdPairRecord_lc8dde4*>(
        self->allocator->vf_0x40((u64)self->count * sizeof(UnkIdPairRecord_lc8dde4), 8, self->flags));
    CStrId* ids = static_cast<CStrId*>(self->allocator->vf_0x40((u64)self->count * sizeof(CStrId), 8, self->flags));
    if (UnkIdPairRecord_lc8dde4* old = self->records) {
        UnkIdPairRecord_lc8dde4* dst = records;
        for (u32 i = 0; i < self->count; ++i, ++dst, ++old) {
            dst->MoveFrom(*old);
            old->second.Reset();
            old->first.Reset();
        }
        self->allocator->vf_0x48(self->records);
    }
    if (CStrId* old = self->ids) {
        CStrId* dst = ids;
        for (u32 i = 0; i < self->count; ++i, ++old, ++dst) {
            new (dst) CStrId(*old);
            old->Reset();
        }
        self->allocator->vf_0x48(self->ids);
    }
    self->records = records;
    self->ids = ids;
    self->capacity = self->count;
}
