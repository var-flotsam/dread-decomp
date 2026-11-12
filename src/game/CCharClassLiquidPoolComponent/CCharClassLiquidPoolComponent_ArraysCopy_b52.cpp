// CCharClassLiquidPoolComponent unit (unit 1394 of tools/tu_map.py): the copy constructor of a pair
// of arrays (CBlackboard.h's UnkPropertyArrays) whose values are 16-byte plain records, fn_7100904b48.
// The storage is reserved (fn_7100904c48, this unit's reserve), the values copied word by word,
// then the keys copied, counting a use of each. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CBlackboard.h"
#include "game/CCharClassLiquidPoolComponent.h"
#include "types.h"

using base::global::UnkStringData;

// Copied through word accesses that may alias anything: the original reloads both arrays and the
// count after every element.
typedef u32 __attribute__((may_alias)) UnkAliasU32_b52;

struct UnkLiquidPoolValue_b52 {
    UnkAliasU32_b52 field_0x0;
    UnkAliasU32_b52 field_0x4;
    UnkAliasU32_b52 field_0x8;
    UnkAliasU32_b52 field_0xc;
};

void fn_7100904b48(UnkPropertyArrays* self, const UnkPropertyArrays* other) {
    self->ids = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->values = nullptr;
    self->step = other->step;
    self->flags = other->flags;
    self->allocator = other->allocator;
    fn_7100904c48(self, other->count);
    self->count = other->count;
    for (u64 i = 0; i < self->count; ++i) {
        UnkAliasU32_b52* dst = reinterpret_cast<UnkAliasU32_b52*>(&static_cast<UnkLiquidPoolValue_b52*>(self->values)[i]);
        const UnkAliasU32_b52* src =
            reinterpret_cast<const UnkAliasU32_b52*>(&static_cast<const UnkLiquidPoolValue_b52*>(other->values)[i]);
        dst[0] = src[0];
        dst[1] = src[1];
        dst[2] = src[2];
        dst[3] = src[3];
    }
    self->count = other->count;
    for (u64 i = 0; i < self->count; ++i) {
        UnkStringData* data = other->ids[i].data;
        __builtin_memcpy(&self->ids[i], &data, sizeof(data));
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
}

// The reserve: larger arrays are allocated, the values copied word by word, the keys moved over
// (the old one left empty, then released), and the old arrays freed.
void fn_7100904c48(UnkPropertyArrays* self, u32 capacity) {
    if (self->capacity < capacity) {
        UnkLiquidPoolValue_b52* values = static_cast<UnkLiquidPoolValue_b52*>(
            self->allocator->vf_0x40((u64)capacity * sizeof(UnkLiquidPoolValue_b52), 8, self->flags));
        base::global::CStrId* ids = static_cast<base::global::CStrId*>(
            self->allocator->vf_0x40((u64)capacity * sizeof(base::global::CStrId), 8, self->flags));
        if (self->values) {
            UnkLiquidPoolValue_b52* from = static_cast<UnkLiquidPoolValue_b52*>(self->values);
            UnkLiquidPoolValue_b52* to = values;
            for (u32 i = 0; i < self->count; ++i, ++to, ++from) {
                UnkAliasU32_b52* dst = reinterpret_cast<UnkAliasU32_b52*>(to);
                const UnkAliasU32_b52* src = reinterpret_cast<const UnkAliasU32_b52*>(from);
                dst[0] = src[0];
                dst[1] = src[1];
                dst[2] = src[2];
                dst[3] = src[3];
            }
            self->allocator->vf_0x48(self->values);
        }
        if (base::global::CStrId* old = self->ids) {
            u64 j = 0;
            for (u32 i = 0; i < self->count; ++i, ++j, ++old) {
                __builtin_memcpy(&ids[j], old, sizeof(base::global::CStrId));
                base::global::UnkStringPool* pool = g_unk_7101d47108;
                old->data = &pool->empty;
                if ((pool = g_unk_7101d47108)) {
                    if (old->data != &pool->empty) {
                        --old->data->ref_count;
                        if (old->data->ref_count == 0) {
                            fn_71000803a4(pool, old->data);
                            pool = g_unk_7101d47108;
                        }
                    }
                    old->data = &pool->empty;
                }
            }
            self->allocator->vf_0x48(self->ids);
        }
        self->values = values;
        self->ids = ids;
        self->capacity = capacity;
    }
}
