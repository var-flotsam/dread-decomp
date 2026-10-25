// Move-assignment of a 0xc8-byte record: releases and takes over each string id (leaving the
// pool's empty string), assigns its string with its own storage out of line (fn_710007eaa0)
// and copies the plain values.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

struct UnkReleasedStrId {
    UnkStringData* data;

    __attribute__((always_inline)) ~UnkReleasedStrId() {
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

__attribute__((always_inline)) static inline void MoveId(UnkReleasedStrId* self, UnkReleasedStrId* other) {
    self->~UnkReleasedStrId();
    self->data = other->data;
    other->data = &g_unk_7101d47108->empty;
}

// A string (base::global::CRntString) with its own inline storage behind it.
struct UnkStrBuffer {
    u8 unk_0x0[0x88];
};

// Plain values copied as a whole.
struct UnkWords5 {
    u32 field_0x0[5];
};

struct UnkRecordC8 {
    UnkReleasedStrId id_0x0;
    u64 field_0x8;
    UnkReleasedStrId id_0x10;
    UnkReleasedStrId id_0x18;
    UnkStrBuffer field_0x20;
    u32 field_0xa8;
    u32 field_0xac;
    UnkWords5 field_0xb0;
};
static_assert(sizeof(UnkRecordC8) == 0xc8, "UnkRecordC8 layout");

UnkRecordC8* fn_7100e6e320(UnkRecordC8* self, UnkRecordC8* other) {
    MoveId(&self->id_0x0, &other->id_0x0);
    self->field_0x8 = other->field_0x8;
    MoveId(&self->id_0x10, &other->id_0x10);
    MoveId(&self->id_0x18, &other->id_0x18);
    fn_710007eaa0(reinterpret_cast<base::global::CRntString*>(&self->field_0x20),
                  reinterpret_cast<const base::global::CRntString*>(&other->field_0x20));
    self->field_0xa8 = other->field_0xa8;
    self->field_0xac = other->field_0xac;
    self->field_0xb0 = other->field_0xb0;
    return self;
}
