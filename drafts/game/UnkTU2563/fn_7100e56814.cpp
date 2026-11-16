// CMinimapData's unit (2563): the Reserve of an array of array-pair records with a string id per record
// alongside: the records are moved and destroyed, the ids moved. Names and layouts are placeholders.
#include <new>

#include "base/UnkFreedVectorPair.h"
#include "base/global/CRntSmallDictionary.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkTU2563.h"
#include "types.h"

using base::global::CStrId;

// Records with their ids alongside in a second array.
using UnkPairIdArray_le56814 = base::global::CRntSmallDictionary<CStrId, UnkFreedVectorPair>;

// Grows both arrays to `new_capacity`.
void fn_7100e56814(void* array, u32 new_capacity) {
    UnkPairIdArray_le56814* self = static_cast<UnkPairIdArray_le56814*>(array);
    if (self->capacity < new_capacity) {
        UnkFreedVectorPair* records = static_cast<UnkFreedVectorPair*>(
            self->allocator->vf_0x40((u64)new_capacity * sizeof(UnkFreedVectorPair), 8, self->flags));
        CStrId* ids = static_cast<CStrId*>(self->allocator->vf_0x40((u64)new_capacity * sizeof(CStrId), 8, self->flags));
        if (UnkFreedVectorPair* old = self->values) {
            UnkFreedVectorPair* dst = records;
            for (u32 i = 0; i < self->count; ++i, ++dst, ++old) {
                new (dst) UnkFreedVectorPair(static_cast<UnkFreedVectorPair&&>(*old));
                old->~UnkFreedVectorPair();
            }
            self->allocator->vf_0x48(self->values);
        }
        if (CStrId* old = self->keys) {
            CStrId* dst = ids;
            for (u32 i = 0; i < self->count; ++i, ++old, ++dst) {
                dst->data = old->data;
                old->data = &g_unk_7101d47108->empty;
                old->Reset();
            }
            self->allocator->vf_0x48(self->keys);
        }
        self->values = records;
        self->keys = ids;
        self->capacity = new_capacity;
    }
}
