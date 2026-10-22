// Copies of fn_7100e59f50 (src/game_aca57c/fn_7100d3d424.cpp) for other record types, naming their own callees and data
// (the comments are adapted from the original's): fn_7100018558, fn_710025ff90. Copy-assignments of maps of ids to
// records (values at 0, keys at 8, sharing one count): both sides are destroyed, the storage is reallocated when the
// other's count differs from the capacity, then the values are assigned and the keys copied, counting a use of each.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using namespace base::global;

struct UnkReleasedStrId {
    UnkStringData* data;

    ~UnkReleasedStrId() {
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
    }
};

template <class V>
struct UnkIdMap {
    V* values;
    UnkReleasedStrId* keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;
};

template <class V, void (*Destroy)(V*), void (*Free)(UnkIdMap<V>*), void (*Allocate)(UnkIdMap<V>*, u32),
          V* (*AssignValue)(V*, const V*)>
static inline UnkIdMap<V>* Assign(UnkIdMap<V>* self, const UnkIdMap<V>* other) {
    if (self != other) {
        if (self->keys)
            for (u64 i = 0; i < self->count; ++i)
                self->keys[i].~UnkReleasedStrId();
        if (self->values)
            for (u32 i = 0; i < self->count; ++i)
                Destroy(&self->values[i]);
        self->count = 0;
        if (other->count != self->capacity) {
            Free(self);
            Allocate(self, other->count);
        }
        self->count = other->count;
        for (u32 i = 0; i < self->count; ++i)
            AssignValue(&self->values[i], &other->values[i]);
        self->count = other->count;
        for (u32 i = 0; i < self->count; ++i) {
            UnkStringData* data = other->keys[i].data;
            __builtin_memcpy(&self->keys[i], &data, sizeof(data));
            if (&g_unk_7101d47108->empty != data)
                ++data->ref_count;
        }
    }
    return self;
}

struct UnkRecord80 {
    u64 unk_0x0[0x80 / 8];
};
using UnkIdRecordMap = UnkIdMap<UnkRecord80>;
void fn_7100017ce8(UnkRecord80* record);              // destructor
void fn_710001871c(UnkIdRecordMap* self);             // frees the storage
void fn_7100017948(UnkIdRecordMap* self, u32 capacity);  // Reserve
UnkRecord80* fn_7100017ba0(UnkRecord80* self, const UnkRecord80* other);

UnkIdRecordMap* fn_7100018558(UnkIdRecordMap* self, const UnkIdRecordMap* other) {
    return Assign<UnkRecord80, fn_7100017ce8, fn_710001871c, fn_7100017948, fn_7100017ba0>(self, other);
}

// Ids to 0x60-byte objects destroyed through their vtable.
struct UnkVirtualRecord60 {
    virtual ~UnkVirtualRecord60();
    u64 unk_0x8[0x58 / 8];
};
__attribute__((always_inline)) inline void DestroyVirtual(UnkVirtualRecord60* record) {
    record->~UnkVirtualRecord60();
}
using UnkIdVirtualRecordMap = UnkIdMap<UnkVirtualRecord60>;
void fn_7100260164(UnkIdVirtualRecordMap* self);                // frees the storage
void fn_710025fdf4(UnkIdVirtualRecordMap* self, u32 capacity);  // Reserve
UnkVirtualRecord60* fn_710025f17c(UnkVirtualRecord60* self, const UnkVirtualRecord60* other);

UnkIdVirtualRecordMap* fn_710025ff90(UnkIdVirtualRecordMap* self, const UnkIdVirtualRecordMap* other) {
    return Assign<UnkVirtualRecord60, DestroyVirtual, fn_7100260164, fn_710025fdf4, fn_710025f17c>(self, other);
}
