// CMinimapData's unit (2563): grows an array of 0x80-byte records (an id and an array pair each) with a
// string id per record alongside: the records and the ids are moved over and the old ones released.
// Names and layouts are placeholders.
#include <new>

#include "base/UnkFreedVectorPair.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStrIdBase;

struct UnkIdPairRecord_le60df4 {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xc;
    CStrId id;
    UnkFreedVectorPair pair;
    u32 field_0x78;
};
static_assert(sizeof(UnkIdPairRecord_le60df4) == 0x80, "");

struct UnkIdPairRecordArray_le60df4 {
    UnkIdPairRecord_le60df4* records;
    CStrId* ids;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;
};

void fn_7100e60df4(void* array, u32 new_capacity) {
    UnkIdPairRecordArray_le60df4* self = static_cast<UnkIdPairRecordArray_le60df4*>(array);
    if (self->capacity >= new_capacity)
        return;
    UnkIdPairRecord_le60df4* records = static_cast<UnkIdPairRecord_le60df4*>(
        self->allocator->vf_0x40((u64)new_capacity * sizeof(UnkIdPairRecord_le60df4), 8, self->flags));
    CStrId* ids = static_cast<CStrId*>(self->allocator->vf_0x40((u64)new_capacity * sizeof(CStrId), 8, self->flags));
    if (UnkIdPairRecord_le60df4* old = self->records) {
        UnkIdPairRecord_le60df4* dst = records;
        for (u32 i = 0; i < self->count; ++i, ++dst, ++old) {
            dst->field_0x0 = old->field_0x0;
            dst->field_0x4 = old->field_0x4;
            dst->field_0x8 = old->field_0x8;
            dst->field_0xc = old->field_0xc;
            new (&dst->id) UnkStrIdBase(old->id);
            old->id.data = &g_unk_7101d47108->empty;
            new (&dst->pair) UnkFreedVectorPair(static_cast<UnkFreedVectorPair&&>(old->pair));
            dst->field_0x78 = old->field_0x78;
            old->pair.~UnkFreedVectorPair();
            old->id.Reset();
        }
        self->allocator->vf_0x48(self->records);
    }
    if (CStrId* old = self->ids) {
        CStrId* dst = ids;
        for (u32 i = 0; i < self->count; ++i, ++old, ++dst) {
            CStrId::MoveConstructReloading(dst, *old);
            old->Reset();
        }
        self->allocator->vf_0x48(self->ids);
    }
    self->records = records;
    self->ids = ids;
    self->capacity = new_capacity;
}
