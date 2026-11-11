// Unit 27: the factory registry's keyed array of 0x48-byte entries (the types of
// src/base/UnkResFactoryRegistry.cpp, which calls this). fn_7100022e2c grows both arrays to
// `capacity`: the entries are moved over and destroyed (fn_7100020164), the keys moved and
// released. Names and layouts are placeholders.
#include <new>

#include "base/UnkVector.h"
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
                    fn_71000803a4(pool, data);
                    pool = *Pool;
                }
            }
            data = &pool->empty;
        }
    }
};

struct UnkRegistryDesc;

// Copied member by member (the original reloads the source after each store).
struct UnkRegistryCallback {
    void* context;
    void (*function)();

    UnkRegistryCallback(const UnkRegistryCallback& other) : context(other.context), function(other.function) {}
};

struct UnkRegistryEntryHead {
    UnkRegistryDesc* desc;
    void* factory;
};

struct UnkRegistryEntry : UnkRegistryEntryHead {
    UnkRegistryEntryId<&g_unk_7101d47108> id;
    UnkRegistryEntryId<&g_unk_7101d47200> path_0x18;
    UnkRegistryEntryId<&g_unk_7101d47200> path_0x20;
    UnkRegistryCallback field_0x28;
    UnkRegistryCallback field_0x38;

    UnkRegistryEntry(UnkRegistryEntry&& other) = default;
};
static_assert(sizeof(UnkRegistryEntry) == 0x48, "UnkRegistryEntry layout");

struct UnkRegistryEntries {
    UnkRegistryEntry* values;
    UnkRegistryEntryId<&g_unk_7101d47108>* keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;
};

void fn_7100022e2c(UnkRegistryEntries* self, u32 capacity) {
    using Key = UnkRegistryEntryId<&g_unk_7101d47108>;
    if (self->capacity >= capacity)
        return;
    UnkRegistryEntry* values = static_cast<UnkRegistryEntry*>(
        self->allocator->vf_0x40((u64)capacity * sizeof(UnkRegistryEntry), 8, self->flags));
    Key* keys = static_cast<Key*>(self->allocator->vf_0x40((u64)capacity * sizeof(Key), 8, self->flags));
    if (self->values) {
        UnkRegistryEntry* src = self->values;
        UnkRegistryEntry* dst = values;
        for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
            new (dst) UnkRegistryEntry(static_cast<UnkRegistryEntry&&>(*src));
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
