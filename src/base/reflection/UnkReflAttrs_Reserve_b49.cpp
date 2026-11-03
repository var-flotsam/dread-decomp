// From unit 70 of tools/tu_map.py (the attributes of src/base/reflection/UnkReflAttrs.cpp): the
// reserve of the attribute arrays (CBlackboard.h's UnkPropertyArrays: 0x18-byte reflected values
// and their keys' string ids), fn_7100093dd0. Larger arrays are allocated, each value copied
// (fn_71000a179c) and destroyed (fn_71000a1c60), each key moved over (the old one left empty, then
// released), and the old arrays freed. Names are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CBlackboard.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringPool;
using base::reflection::UnkReflValue;

// As src/base/reflection/UnkReflAttrs.cpp declares it.
void fn_71000a179c(UnkReflValue* self, const UnkReflValue* other);  // copy constructor

void fn_7100093dd0(UnkPropertyArrays* self, u32 capacity) {
    if (self->capacity < capacity) {
        UnkReflValue* values = static_cast<UnkReflValue*>(
            self->allocator->vf_0x40((u64)capacity * sizeof(UnkReflValue), 8, self->flags));
        CStrId* ids = static_cast<CStrId*>(self->allocator->vf_0x40((u64)capacity * sizeof(CStrId), 8, self->flags));
        if (self->values) {
            UnkReflValue* src = static_cast<UnkReflValue*>(self->values);
            UnkReflValue* dst = values;
            for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
                fn_71000a179c(dst, src);
                fn_71000a1c60(src);
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
