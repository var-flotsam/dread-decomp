// CPlayerInfo unit (tools/tu_map.py unit 2592): the flat maps of string ids to values. fn_7100f0eaec erases a
// 0xb0-byte value (UnkPropertyArrays' layout; values copied by fn_710002f588, destroyed by fn_710002f67c),
// fn_7100f0ca9c erases one of the 0x30-byte timed entries (a head and the value map, moved inline,
// destroyed by fn_7100f0e85c), both moving the last entry into the hole; fn_7100f0f7b4 reserves the timed
// map like fn_7100f0f980 (CPlayerInfo_Reserve_b52.cpp). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/UnkKeyedStrIds.h"
#include "game/CBlackboard.h"
#include "types.h"

#include <new>

using base::global::CStrId;

struct UnkPlayerInfoValue_ovb0071 {
    u8 raw[0xb0];
};

void fn_710002f588(void* self, const void* other);
void fn_710002f67c(void* self);

UnkPlayerInfoValue_ovb0071* fn_7100f0eaec(UnkPropertyArrays* self, UnkPlayerInfoValue_ovb0071* it) {
    u32 index = it - static_cast<UnkPlayerInfoValue_ovb0071*>(self->values);
    fn_710002f67c(&static_cast<UnkPlayerInfoValue_ovb0071*>(self->values)[index]);
    self->ids[index].Reset();
    if (self->count - 1 != index) {
        fn_710002f588(&static_cast<UnkPlayerInfoValue_ovb0071*>(self->values)[index],
                      &static_cast<UnkPlayerInfoValue_ovb0071*>(self->values)[self->count - 1]);
        fn_710002f67c(&static_cast<UnkPlayerInfoValue_ovb0071*>(self->values)[self->count - 1]);
        CStrId* ids = self->ids;
        u32 last = self->count - 1;
        ids[index].data = ids[last].data;
        base::global::UnkStringData* empty = &g_unk_7101d47108->empty;
        __builtin_memcpy(&ids[last].data, &empty, sizeof(empty));
        self->ids[self->count - 1].Reset();
    }
    --self->count;
    return &static_cast<UnkPlayerInfoValue_ovb0071*>(self->values)[index];
}

struct UnkTimedHead_ovb0071 {
    u8 counter;
    f32 time;
};

struct UnkTimedEntry_ovb0071 {
    UnkTimedHead_ovb0071 head;
    UnkKeyedStrIds properties;  // 0x8

    UnkTimedEntry_ovb0071(UnkTimedEntry_ovb0071&& other)
        : head(other.head), properties(static_cast<UnkKeyedStrIds&&>(other.properties)) {}
};

struct UnkTimedMap_ovb0071 {
    UnkTimedEntry_ovb0071* values;
    CStrId* ids;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;
};

void fn_7100f0e85c(UnkTimedEntry_ovb0071* self);

UnkTimedEntry_ovb0071* fn_7100f0ca9c(UnkTimedMap_ovb0071* self, UnkTimedEntry_ovb0071* it) {
    u32 index = it - self->values;
    fn_7100f0e85c(&self->values[index]);
    self->ids[index].Reset();
    if (self->count - 1 != index) {
        new (&self->values[index]) UnkTimedEntry_ovb0071(static_cast<UnkTimedEntry_ovb0071&&>(self->values[self->count - 1]));
        fn_7100f0e85c(&self->values[self->count - 1]);
        CStrId* ids = self->ids;
        u32 last = self->count - 1;
        ids[index].data = ids[last].data;
        base::global::UnkStringData* empty = &g_unk_7101d47108->empty;
        __builtin_memcpy(&ids[last].data, &empty, sizeof(empty));
        self->ids[self->count - 1].Reset();
    }
    --self->count;
    return &self->values[index];
}

void fn_7100f0f7b4(UnkTimedMap_ovb0071* self, u32 capacity) {
    if (self->capacity < capacity) {
        UnkTimedEntry_ovb0071* values = static_cast<UnkTimedEntry_ovb0071*>(
            self->allocator->vf_0x40((u64)capacity * sizeof(UnkTimedEntry_ovb0071), 8, self->flags));
        CStrId* ids = static_cast<CStrId*>(self->allocator->vf_0x40((u64)capacity * sizeof(CStrId), 8, self->flags));
        if (self->values) {
            UnkTimedEntry_ovb0071* src = self->values;
            UnkTimedEntry_ovb0071* dst = values;
            for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
                new (dst) UnkTimedEntry_ovb0071(static_cast<UnkTimedEntry_ovb0071&&>(*src));
                fn_7100f0e85c(src);
            }
            self->allocator->vf_0x48(self->values);
        }
        if (CStrId* old = self->ids) {
            u64 j = 0;
            for (u32 i = 0; i < self->count; ++i, ++j, ++old) {
                __builtin_memcpy(&ids[j], old, sizeof(CStrId));
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
