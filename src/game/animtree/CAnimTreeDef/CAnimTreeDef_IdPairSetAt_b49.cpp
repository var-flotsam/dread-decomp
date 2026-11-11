// animtree::CAnimTreeDef's unit: the "set element at index" functions (slots 0x140 and 0x148 of
// the reflected collection type) of an array (base::UnkVector's layout) of pairs of string ids, in
// the shape of src/game/CLightsDef/CLightsDef_b42.cpp: an index past the end of a growable array
// first reserves (fn_71002da674) and constructs the new pairs (zeroed, then fn_71002dacc0), then
// the pair is assigned member by member. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/animtree/CAnimTreeDef.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

// The moved-from id is cleared through a union, so the pool is reloaded after the store.
union UnkAnimTreeIdSlot_b49a1 {
    UnkStringData* p;
};

struct UnkAnimTreeId_b49a1 {
    UnkStringData* data;

    UnkAnimTreeId_b49a1& operator=(const UnkAnimTreeId_b49a1& other) {
        if (data != other.data) {
            UnkStringData* d = other.data;
            UnkStringPool* pool = g_unk_7101d47108;
            if (d != &pool->empty)
                ++d->ref_count;
            if (data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0)
                    fn_71000803a4(pool, data);
            }
            data = d;
        }
        return *this;
    }

    UnkAnimTreeId_b49a1& operator=(UnkAnimTreeId_b49a1&& other) {
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
        reinterpret_cast<UnkAnimTreeIdSlot_b49a1*>(&data)->p = other.data;
        other.data = &g_unk_7101d47108->empty;
        return *this;
    }
};

// Both ids empty; signature as src/game/animtree/CAnimTreeDef/fn_71002dacc0.cpp defines it.
struct UnkIdPair;
void fn_71002dacc0(UnkIdPair* self);

struct UnkAnimTreeIdPair_b49a1 {
    UnkAnimTreeId_b49a1 first;
    UnkAnimTreeId_b49a1 second;
};

struct UnkAnimTreeIdPairs_b49a1 {
    UnkAnimTreeIdPair_b49a1* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
};

static inline void Grow_b49a1(UnkAnimTreeIdPairs_b49a1* self, u32 index) {
    u32 capacity = self->capacity;
    if (capacity <= index && self->step != 0)
        capacity = (index / self->step + 1) * self->step;
    fn_71002da674(self, capacity);
    u32 n = index + 1;
    fn_71002da674(self, n);
    for (u32 i = self->count; i < n; ++i) {
        UnkAnimTreeIdPair_b49a1* p = &self->data[i];
        __builtin_memset(p, 0, sizeof(*p));
        fn_71002dacc0(reinterpret_cast<UnkIdPair*>(p));
    }
    self->count = n;
}

void fn_71002da0e0(UnkAnimTreeIdPairs_b49a1* self, const u32* index, const UnkAnimTreeIdPair_b49a1* value) {
    u32 i = *index;
    if (self->field_0x14 && self->count <= i)
        Grow_b49a1(self, i);
    self->data[i] = *value;
}

void fn_71002da258(UnkAnimTreeIdPairs_b49a1* self, const u32* index, UnkAnimTreeIdPair_b49a1* value) {
    u32 i = *index;
    if (self->field_0x14 && self->count <= i)
        Grow_b49a1(self, i);
    self->data[i] = static_cast<UnkAnimTreeIdPair_b49a1&&>(*value);
}
