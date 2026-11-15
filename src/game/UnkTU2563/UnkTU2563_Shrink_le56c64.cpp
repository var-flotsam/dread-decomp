// CMinimapData's unit (2563): shrinks an array of array-pair records with a string id per record alongside
// to its count: the records are moved and destroyed, the ids copied and released. Names and layouts are
// placeholders.
#include <new>

#include "base/UnkFreedVectorPair.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

// Records with their ids alongside in a second array.
struct UnkPairIdArray_le56c64 {
    UnkFreedVectorPair* records;
    CStrId* ids;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;
};

// The array's type as its caller (src/game/UnkTU2563/UnkTU2563_Records_ovb0223.cpp) declares it.
struct UnkRec60IdArray_b0223;

void fn_7100e56c64(UnkRec60IdArray_b0223* array) {
    UnkPairIdArray_le56c64* self = reinterpret_cast<UnkPairIdArray_le56c64*>(array);
    if (self->count == self->capacity)
        return;
    UnkFreedVectorPair* records = static_cast<UnkFreedVectorPair*>(
        self->allocator->vf_0x40((u64)self->count * sizeof(UnkFreedVectorPair), 8, self->flags));
    CStrId* ids = static_cast<CStrId*>(self->allocator->vf_0x40((u64)self->count * sizeof(CStrId), 8, self->flags));
    if (UnkFreedVectorPair* old = self->records) {
        UnkFreedVectorPair* dst = records;
        for (u32 i = 0; i < self->count; ++i, ++dst, ++old) {
            new (dst) UnkFreedVectorPair(static_cast<UnkFreedVectorPair&&>(*old));
            old->~UnkFreedVectorPair();
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
