// Unit 24 (base::core::CAsset): reserves room for `capacity` entries in a map of file path ids to id
// and path pairs (the map fn_710001810c copies): new storage for the values and the keys, each one
// moved over and the old one released, then the old storage freed. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStrIdBase;
using base::global::UnkStringData;
using base::global::UnkStringPool;

struct UnkIdPathValue_b45a1 {
    UnkStringData* id;
    UnkStringData* path;
};

struct UnkPathIdPairMap_b45a1 {
    UnkIdPathValue_b45a1* values;
    UnkStringData** keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;
};

// A string id of the given pool as the map moves it: a move takes the string and leaves the
// pool's empty one; the destructor releases it.
template <UnkStringPool*& Pool>
struct UnkPooledId_l018244 : UnkStrIdBase {
    UnkPooledId_l018244(UnkPooledId_l018244&& other) : UnkStrIdBase(other) { other.data = &Pool->empty; }
    ~UnkPooledId_l018244() {
        if (UnkStringPool* pool = Pool) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = Pool;
                }
            }
            data = &pool->empty;
        }
    }
};
using UnkId_l018244 = UnkPooledId_l018244<g_unk_7101d47108>;
using UnkPathId_l018244 = UnkPooledId_l018244<g_unk_7101d47200>;

// A value as the map moves it.
struct UnkIdPathPair_l018244 {
    UnkId_l018244 id;
    UnkPathId_l018244 path;
};

void fn_7100018244(UnkPathIdPairMap_b45a1* self, u32 capacity) {
    if (self->capacity < capacity) {
        UnkIdPathPair_l018244* values = static_cast<UnkIdPathPair_l018244*>(
            self->allocator->vf_0x40((u64)capacity * sizeof(UnkIdPathPair_l018244), 8, self->flags));
        UnkPathId_l018244* keys = static_cast<UnkPathId_l018244*>(
            self->allocator->vf_0x40((u64)capacity * sizeof(UnkPathId_l018244), 8, self->flags));
        if (self->values) {
            UnkIdPathPair_l018244* old = reinterpret_cast<UnkIdPathPair_l018244*>(self->values);
            UnkIdPathPair_l018244* dst = values;
            for (u32 i = 0; i < self->count; ++i, ++old, ++dst) {
                new (dst) UnkIdPathPair_l018244(static_cast<UnkIdPathPair_l018244&&>(*old));
                old->~UnkIdPathPair_l018244();
            }
            self->allocator->vf_0x48(self->values);
        }
        if (self->keys) {
            UnkPathId_l018244* old = reinterpret_cast<UnkPathId_l018244*>(self->keys);
            for (u64 i = 0; i < self->count; ++i, ++old) {
                new (&keys[i]) UnkPathId_l018244(static_cast<UnkPathId_l018244&&>(*old));
                old->~UnkPathId_l018244();
            }
            self->allocator->vf_0x48(self->keys);
        }
        self->values = reinterpret_cast<UnkIdPathValue_b45a1*>(values);
        self->keys = reinterpret_cast<UnkStringData**>(keys);
        self->capacity = capacity;
    }
}
