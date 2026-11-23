// Unit 27: the factory registry's keyed array of 0x48-byte entries (the types of
// src/base/UnkResFactoryRegistry.cpp, which calls this). fn_7100022e2c grows both arrays to
// `capacity`: the entries are moved over and destroyed (fn_7100020164), the keys moved and
// released. Names and layouts are placeholders.
#include <new>

#include "base/UnkRegistryCallback.h"
#include "base/UnkRegistryEntryHead.h"
#include "base/global/CRntSmallDictionary.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkTU27.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

// The moved-from id is cleared through a union, so its pool is reloaded after the store.
union UnkRegistryIdSlot_b49 {
    UnkStringData* p;
};

// As UnkResFactoryRegistry.cpp's UnkRegistryEntryId: released in two steps.
template <UnkStringPool** Pool>
struct UnkRegistryEntryId {
    UnkStringData* data;

    UnkRegistryEntryId(UnkRegistryEntryId&& other) : data(other.data) {
        reinterpret_cast<UnkRegistryIdSlot_b49*>(&other.data)->p = &(*Pool)->empty;
    }
    ~UnkRegistryEntryId() {
        if (UnkStringPool* pool = *Pool) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    StringPool_Release(pool, data);
                    pool = *Pool;
                }
            }
            data = &pool->empty;
        }
    }
};

struct UnkRegistryDesc;

struct UnkRegistryEntry_b49 : UnkRegistryEntryHead {
    UnkRegistryEntryId<&g_unk_7101d47108> id;
    UnkRegistryEntryId<&g_unk_7101d47200> path_0x18;
    UnkRegistryEntryId<&g_unk_7101d47200> path_0x20;
    UnkRegistryCallback field_0x28;
    UnkRegistryCallback field_0x38;

    UnkRegistryEntry_b49(UnkRegistryEntry_b49&& other) = default;
};
static_assert(sizeof(UnkRegistryEntry_b49) == 0x48, "UnkRegistryEntry_b49 layout");

using UnkRegistryEntries = base::global::CRntSmallDictionary<UnkRegistryEntryId<&g_unk_7101d47108>, UnkRegistryEntry_b49>;

void fn_7100022e2c(UnkRegistryEntries* self, u32 capacity) {
    using Key = UnkRegistryEntryId<&g_unk_7101d47108>;
    if (self->capacity >= capacity)
        return;
    UnkRegistryEntry_b49* values = static_cast<UnkRegistryEntry_b49*>(
        self->allocator->vf_0x40((u64)capacity * sizeof(UnkRegistryEntry_b49), 8, self->flags));
    Key* keys = static_cast<Key*>(self->allocator->vf_0x40((u64)capacity * sizeof(Key), 8, self->flags));
    if (self->values) {
        UnkRegistryEntry_b49* src = self->values;
        UnkRegistryEntry_b49* dst = values;
        for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
            new (dst) UnkRegistryEntry_b49(static_cast<UnkRegistryEntry_b49&&>(*src));
            fn_7100020164(reinterpret_cast<char*>(src));
        }
        self->allocator->vf_0x48(self->values);
    }
    if (self->keys) {
        Key* src = self->keys;
        u64 j = 0;
        for (u32 i = 0; i < self->count; ++i, ++j, ++src) {
            new (&keys[j]) Key(static_cast<Key&&>(*src));
            src->~Key();
        }
        self->allocator->vf_0x48(self->keys);
    }
    self->values = values;
    self->keys = keys;
    self->capacity = capacity;
}
