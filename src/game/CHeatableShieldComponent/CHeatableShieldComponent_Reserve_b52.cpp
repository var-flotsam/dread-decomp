// Unit 1322 of tools/tu_map.py (CHeatableShieldComponent): the reserve of a pair of attribute
// arrays (CBlackboard.h's UnkPropertyArrays) whose values are this unit's own 0xd0-byte type,
// copied with fn_710086dbb0 and destroyed with fn_710086e0ac; a copy of fn_7100093dd0.
// Names are placeholders.
#include "base/global/CStrId.h"
#include "game/CBlackboard.h"
#include "game/UnkTU1323.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringPool;

struct UnkHeatableValue_0xd0 {
    u8 raw[0xd0];
};

void fn_710086dbb0(UnkHeatableValue_0xd0* self, const UnkHeatableValue_0xd0* other);  // copy constructor

void fn_710086da20(UnkPropertyArrays* self, u32 capacity) {
    if (self->capacity < capacity) {
        UnkHeatableValue_0xd0* values = static_cast<UnkHeatableValue_0xd0*>(
            self->allocator->vf_0x40((u64)capacity * sizeof(UnkHeatableValue_0xd0), 8, self->flags));
        CStrId* ids = static_cast<CStrId*>(self->allocator->vf_0x40((u64)capacity * sizeof(CStrId), 8, self->flags));
        if (self->values) {
            UnkHeatableValue_0xd0* src = static_cast<UnkHeatableValue_0xd0*>(self->values);
            UnkHeatableValue_0xd0* dst = values;
            for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
                fn_710086dbb0(dst, src);
                fn_710086e0ac(src);
            }
            self->allocator->vf_0x48(self->values);
        }
        if (CStrId* old = self->ids) {
            u64 j = 0;
            for (u32 i = 0; i < self->count; ++i, ++j, ++old) {
                __builtin_memcpy(&ids[j], old, sizeof(CStrId));
                UnkStringPool* pool = g_unk_7101d47108;
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
