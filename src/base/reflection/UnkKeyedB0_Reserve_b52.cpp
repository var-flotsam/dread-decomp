// Unit 83 of tools/tu_map.py (base::reflection): the reserve of a keyed array of 0xb0-byte records with
// string-id keys, a copy of fn_7100093dd0 (src/base/reflection/UnkReflAttrs_Reserve_b49.cpp) for the
// records of src/base/reflection/UnkReflKeyedB0_b44.cpp: each record moved (fn_7100097d18) and the old
// one destroyed (fn_71000e70c4), each key moved over and the old one released. Names are placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringPool;

struct UnkRecordB0_b44a1;  // the record as src/base/reflection/UnkReflKeyedB0_b44.cpp names it
void fn_71000e70c4(UnkRecordB0_b44a1* self);
// As src/game_s1/fn_71000dbc34.cpp declares it.
void fn_7100097d18(void* self, void* other);

namespace {
struct RecordB0 {
    u64 words[0xb0 / 8];
};
}  // namespace

struct UnkKeyedB0Arrays_b52 {
    void* values;
    CStrId* ids;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;
};

static void Reserve(UnkKeyedB0Arrays_b52* self, u32 capacity) {
    if (self->capacity < capacity) {
        RecordB0* values =
            static_cast<RecordB0*>(self->allocator->vf_0x40((u64)capacity * sizeof(RecordB0), 8, self->flags));
        CStrId* ids = static_cast<CStrId*>(self->allocator->vf_0x40((u64)capacity * sizeof(CStrId), 8, self->flags));
        if (self->values) {
            RecordB0* src = static_cast<RecordB0*>(self->values);
            RecordB0* dst = values;
            for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
                fn_7100097d18(dst, src);
                fn_71000e70c4(reinterpret_cast<UnkRecordB0_b44a1*>(src));
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

// As src/clones/clones_00.cpp declares it.
void fn_71000dcd2c(void* self, u32 capacity) {
    Reserve(static_cast<UnkKeyedB0Arrays_b52*>(self), capacity);
}
