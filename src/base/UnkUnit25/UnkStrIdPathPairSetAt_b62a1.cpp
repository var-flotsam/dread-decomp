// Unit 25: the "set element at index" functions (slots 0x140 and 0x148 of the reflected collection
// type) of an array (base::UnkVector's layout) of string id and file-path id pairs, in the shape of
// src/game/animtree/CAnimTreeDef/CAnimTreeDef_IdPairSetAt_b49.cpp: an index past the end of a
// growable array first reserves (fn_710001e4e8) and constructs the new pairs as copies of the empty
// ids (g_unk_7101d470e0, g_unk_7101d470e8), then the pair is assigned member by member. Names and
// layouts are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

// The moved-from id is cleared through a union, so the pool is reloaded after the store.
union UnkIdSlot_b62a1 {
    UnkStringData* p;
};

template <UnkStringPool*& Pool>
inline void CopyAssignId_b62a1(UnkStringData*& data, UnkStringData* const& other) {
    if (data != other) {
        UnkStringData* d = other;
        UnkStringPool* pool = Pool;
        if (d != &pool->empty)
            ++d->ref_count;
        if (data != &pool->empty) {
            --data->ref_count;
            if (data->ref_count == 0)
                fn_71000803a4(pool, data);
        }
        data = d;
    }
}

struct UnkPairId_b62a1 : base::global::CStrId {
    UnkPairId_b62a1() : CStrId(g_unk_7101d470e0) {}
    UnkPairId_b62a1& operator=(const UnkPairId_b62a1& other) {
        CStrId::operator=(other);
        return *this;
    }
    UnkPairId_b62a1& operator=(UnkPairId_b62a1&& other) {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
        reinterpret_cast<UnkIdSlot_b62a1*>(&data)->p = other.data;
        other.data = &g_unk_7101d47108->empty;
        return *this;
    }
};

struct UnkPairPath_b62a1 : base::global::UnkFilePathStrIdBase {
    UnkPairPath_b62a1() {}
    UnkPairPath_b62a1& operator=(const UnkPairPath_b62a1& other) {
        CopyAssignId_b62a1<g_unk_7101d47200>(data, other.data);
        return *this;
    }
    UnkPairPath_b62a1& operator=(UnkPairPath_b62a1&& other) {
        UnkStringPool* pool = g_unk_7101d47200;
        if (pool) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47200;
                }
            }
            data = &pool->empty;
        }
        data = other.data;
        other.data = &pool->empty;
        return *this;
    }
};

struct UnkIdPathPair_b62a1 {
    UnkPairId_b62a1 id;
    UnkPairPath_b62a1 path;

    // The path is copied from the empty path id in the body (its load comes before the member's
    // address).
    UnkIdPathPair_b62a1() {
        path.data = g_unk_7101d470e8.data;
        if (&g_unk_7101d47200->empty != path.data)
            ++path.data->ref_count;
    }
};

struct UnkIdPathPairs_b62a1 {
    UnkIdPathPair_b62a1* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
};

// Signature as src/base/UnkUnit25/UnkStrIdPathPairVecReserve_b45.cpp defines it.
struct UnkStrIdPathPairVec_b45a1;
void fn_710001e4e8(UnkStrIdPathPairVec_b45a1* self, u32 capacity);  // reserve

static inline void Grow_b62a1(UnkIdPathPairs_b62a1* self, u32 index) {
    u32 capacity = self->capacity;
    if (capacity <= index && self->step != 0)
        capacity = (index / self->step + 1) * self->step;
    fn_710001e4e8(reinterpret_cast<UnkStrIdPathPairVec_b45a1*>(self), capacity);
    u32 n = index + 1;
    fn_710001e4e8(reinterpret_cast<UnkStrIdPathPairVec_b45a1*>(self), n);
    for (u32 i = self->count; i < n; ++i)
        new (&self->data[i]) UnkIdPathPair_b62a1();
    self->count = n;
}

void fn_710001de1c(UnkIdPathPairs_b62a1* self, const u32* index, const UnkIdPathPair_b62a1* value) {
    u32 i = *index;
    if (self->field_0x14 && self->count <= i)
        Grow_b62a1(self, i);
    self->data[i] = *value;
}

void fn_710001dffc(UnkIdPathPairs_b62a1* self, const u32* index, UnkIdPathPair_b62a1* value) {
    u32 i = *index;
    if (self->field_0x14 && self->count <= i)
        Grow_b62a1(self, i);
    self->data[i] = static_cast<UnkIdPathPair_b62a1&&>(*value);
}
