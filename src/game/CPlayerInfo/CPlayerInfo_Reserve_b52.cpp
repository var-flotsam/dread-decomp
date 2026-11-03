// CPlayerInfo unit (tools/tu_map.py unit 2592): a copy of fn_7100093dd0 (UnkReflAttrs_Reserve_b49.cpp),
// the reserve of a pair of arrays (UnkPropertyArrays' layout: 0xb0-byte values and their keys' string
// ids). Each value is copied (fn_710002f588) and destroyed (fn_710002f67c), each key moved over (the
// old one left empty, then released), and the old arrays freed. Names are placeholders.
#include "base/global/CStrId.h"
#include "game/CBlackboard.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringPool;

struct UnkPlayerInfoValue_b52 {
    u8 raw[0xb0];
};

void fn_710002f588(void* self, const void* other);  // copy constructor
// As src/game/UnkUnit1134/UnkUnit1134_Dtor_b49a3.cpp declares it.
void fn_710002f67c(void* self);

void fn_7100f0f980(UnkPropertyArrays* self, u32 capacity) {
    if (self->capacity < capacity) {
        UnkPlayerInfoValue_b52* values = static_cast<UnkPlayerInfoValue_b52*>(
            self->allocator->vf_0x40((u64)capacity * sizeof(UnkPlayerInfoValue_b52), 8, self->flags));
        CStrId* ids = static_cast<CStrId*>(self->allocator->vf_0x40((u64)capacity * sizeof(CStrId), 8, self->flags));
        if (self->values) {
            UnkPlayerInfoValue_b52* src = static_cast<UnkPlayerInfoValue_b52*>(self->values);
            UnkPlayerInfoValue_b52* dst = values;
            for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
                fn_710002f588(dst, src);
                fn_710002f67c(src);
            }
            self->allocator->vf_0x48(self->values);
        }
        if (CStrId* old = self->ids) {
            // A 64-bit index for the new array beside the 32-bit counter (as fn_710000542c).
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
