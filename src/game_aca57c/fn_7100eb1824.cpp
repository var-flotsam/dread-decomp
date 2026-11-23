// A container of two parallel arrays (arrays of string ids with two values each, and string
// ids) sharing a count and capacity: its clear (destroys the ids, then destroys and frees each
// inner array), shrink-to-fit and reserve. Both reallocations move the inner arrays over; the
// shrink copies the ids and destroys the old ones, the reserve moves them.
#include "base/global/CRntSmallDictionary.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using namespace base::global;

// A string id whose destructor decrements and tests in two steps (the original reloads the id
// after the decrement).
struct UnkReleasedStrId {
    UnkStringData* data;

    UnkReleasedStrId(const UnkReleasedStrId& other) : data(other.data) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    UnkReleasedStrId(UnkReleasedStrId&& other) : data(other.data) { other.data = &g_unk_7101d47108->empty; }

    ~UnkReleasedStrId() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    StringPool_Release(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

struct UnkIdValues {
    UnkReleasedStrId id;
    u32 field_0x8;
    u32 field_0xc;
};

using UnkMovedIdValuesVector = base::global::CRntVector<UnkIdValues>;

using UnkIdValuesArrays = base::global::CRntSmallDictionary<UnkReleasedStrId, base::global::CRntVector<UnkIdValues>>;

void fn_7100eb1824(UnkIdValuesArrays* self) {
    if (self->keys) {
        for (u64 i = 0; i < self->count; ++i)
            self->keys[i].~UnkReleasedStrId();
    }
    if (self->values) {
        for (u64 i = 0; i < self->count; ++i)
            self->values[i].Free();
    }
    self->count = 0;
}

// Moves the inner arrays to `records`, freeing the old array.
static inline void MoveRecords(UnkIdValuesArrays* self, UnkMovedIdValuesVector* records) {
    if (UnkMovedIdValuesVector* old = reinterpret_cast<UnkMovedIdValuesVector*>(self->values)) {
        UnkMovedIdValuesVector* dst = records;
        for (u32 i = 0; i < self->count; ++i, ++old, ++dst)
            new (dst) UnkMovedIdValuesVector(static_cast<UnkMovedIdValuesVector&&>(*old));
        self->allocator->vf_0x48(self->values);
    }
}

// NON_MATCHING(minor): in the ids loop the original steps a pointer to the old id alongside the
// index; ours addresses it by the index.
void fn_7100eb166c(UnkIdValuesArrays* self) {
    if (self->count == self->capacity)
        return;
    auto* records = static_cast<UnkMovedIdValuesVector*>(
        self->allocator->vf_0x40((u64)self->count * sizeof(UnkMovedIdValuesVector), 8, self->flags));
    auto* ids = static_cast<UnkReleasedStrId*>(
        self->allocator->vf_0x40((u64)self->count * sizeof(UnkReleasedStrId), 8, self->flags));
    MoveRecords(self, records);
    if (UnkReleasedStrId* old = self->keys) {
        UnkReleasedStrId* dst = ids;
        for (u32 i = 0; i < self->count; ++i, ++old, ++dst) {
            new (dst) UnkReleasedStrId(*old);
            old->~UnkReleasedStrId();
        }
        self->allocator->vf_0x48(self->keys);
    }
    self->values = reinterpret_cast<base::global::CRntVector<UnkIdValues>*>(records);
    self->keys = ids;
    self->capacity = self->count;
}

void fn_7100b48944(UnkIdValuesArrays* self, u32 n) {
    if (self->capacity >= n)
        return;
    auto* records = static_cast<UnkMovedIdValuesVector*>(
        self->allocator->vf_0x40((u64)n * sizeof(UnkMovedIdValuesVector), 8, self->flags));
    auto* ids = static_cast<UnkReleasedStrId*>(self->allocator->vf_0x40((u64)n * sizeof(UnkReleasedStrId), 8, self->flags));
    MoveRecords(self, records);
    if (UnkReleasedStrId* old = self->keys) {
        UnkReleasedStrId* dst = ids;
        for (u32 i = 0; i < self->count; ++i, ++old, ++dst) {
            new (dst) UnkReleasedStrId(static_cast<UnkReleasedStrId&&>(*old));
            old->~UnkReleasedStrId();
        }
        self->allocator->vf_0x48(self->keys);
    }
    self->values = reinterpret_cast<base::global::CRntVector<UnkIdValues>*>(records);
    self->keys = ids;
    self->capacity = n;
}
