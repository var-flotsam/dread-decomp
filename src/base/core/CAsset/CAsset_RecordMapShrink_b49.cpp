// Unit 24 (base::core::CAsset): the map of string ids to 0x80-byte records (the types of
// src/base/UnkAssetRecordMap.cpp and src/base/core/CAsset/fn_7100018558.cpp). fn_710001871c
// reallocates both arrays to the current count: the records are moved over and destroyed, the
// keys copied (counting a use) and released. Names and layouts are placeholders.

#include <new>

#include "base/UnkDestructors.h"
#include "base/UnkStrIdPathPair.h"
#include "base/UnkVector.h"
#include "base/core/CAsset.h"
#include "base/global/CFilePathStrId.h"
#include "types.h"

using UnkAssetStrId = base::UnkStrIdMember;
using UnkAssetPathId = base::UnkStrIdMemberOf<g_unk_7101d47200>;

// As src/base/UnkAssetRecordMap.cpp defines them.
template <class V, class K>
struct UnkAssetKeyedMap {
    V* values;
    K* keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;

    UnkAssetKeyedMap(UnkAssetKeyedMap&& other)
        : values(other.values), keys(other.keys), count(other.count), capacity(other.capacity),
          step(other.step), flags(other.flags), allocator(other.allocator) {
        other.count = 0;
        other.values = nullptr;
        other.keys = nullptr;
    }
};

struct UnkAssetPathIdVector {
    UnkAssetPathId* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    UnkAssetPathIdVector(UnkAssetPathIdVector&& other)
        : data(other.data), count(other.count), capacity(other.capacity), step(other.step),
          field_0x14(other.field_0x14), flags(other.flags), allocator(other.allocator) {
        other.data = nullptr;
        other.count = 0;
        other.capacity = 0;
    }
};

struct UnkRecord80 {
    UnkAssetKeyedMap<UnkStrIdPathPair, UnkAssetPathId> field_0x0;
    UnkAssetKeyedMap<UnkStrIdPathPair, UnkAssetPathId> field_0x28;
    UnkAssetPathIdVector field_0x50;
    UnkAssetPathId field_0x78;

    UnkRecord80(UnkRecord80&& other) = default;
};
static_assert(sizeof(UnkRecord80) == 0x80, "UnkRecord80 layout");

template <class V>
struct UnkIdMap {
    V* values;
    UnkAssetStrId* keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;
};

void fn_710001871c(UnkIdMap<UnkRecord80>* self) {
    if (self->count != self->capacity) {
        UnkRecord80* values = static_cast<UnkRecord80*>(
            self->allocator->vf_0x40((u64)self->count * sizeof(UnkRecord80), 8, self->flags));
        UnkAssetStrId* keys = static_cast<UnkAssetStrId*>(
            self->allocator->vf_0x40((u64)self->count * sizeof(UnkAssetStrId), 8, self->flags));
        if (self->values) {
            UnkRecord80* src = self->values;
            UnkRecord80* dst = values;
            for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
                new (dst) UnkRecord80(static_cast<UnkRecord80&&>(*src));
                fn_7100017ce8(src);
            }
            self->allocator->vf_0x48(self->values);
        }
        if (self->keys) {
            // A 64-bit index for the new array beside the 32-bit counter: the end test compares
            // 32 bits (as fn_710000542c).
            UnkAssetStrId* src = self->keys;
            u64 j = 0;
            for (u32 i = 0; i < self->count; ++i, ++j, ++src) {
                asm("" : "+r"(src));  // the old keys are walked by pointer
                base::global::UnkStringData* data = src->data;
                keys[j].data = data;
                if (&g_unk_7101d47108->empty != data)
                    ++data->ref_count;
                src->~UnkAssetStrId();
            }
            self->allocator->vf_0x48(self->keys);
        }
        self->values = values;
        self->keys = keys;
        self->capacity = self->count;
    }
}
