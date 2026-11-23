// Reserve of a container of two parallel arrays (0x120-byte records and pointer-sized ids)
// sharing a count and capacity: when the capacity is below `n`, both are reallocated to `n`,
// the records moved over (each source destroyed by fn_7100b8b7e8), the ids copied and the old
// arrays freed.
#include "base/global/CRntSmallDictionary.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/global/UnkTwoStepStrId.h"
#include "game/UnkMovedArray.h"
#include "game/UnkTU1728.h"
#include "types.h"

using base::UnkAllocator;
using base::global::UnkStringData;

// Two parallel arrays; a move leaves the capacity behind.
struct UnkMovedPairArray {
    void* data;
    void* ids;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    UnkAllocator* allocator;

    UnkMovedPairArray(UnkMovedPairArray&& other)
        : data(other.data), ids(other.ids), count(other.count), capacity(other.capacity),
          step(other.step), flags(other.flags), allocator(other.allocator) {
        other.data = nullptr;
        other.ids = nullptr;
        other.count = 0;
    }
};

struct UnkRecord120 {
    UnkMovedStrId field_0x0;
    UnkMovedArray field_0x8;
    UnkMovedPairArray field_0x30;
    UnkMovedArray field_0x58;
    UnkMovedArray field_0x80;
    UnkMovedArray field_0xa8;
    UnkMovedArray field_0xd0;
    UnkMovedArray field_0xf8;

    __attribute__((always_inline)) UnkRecord120(UnkRecord120&& other)
        : field_0x0(static_cast<UnkMovedStrId&&>(other.field_0x0)),
          field_0x8(static_cast<UnkMovedArray&&>(other.field_0x8)),
          field_0x30(static_cast<UnkMovedPairArray&&>(other.field_0x30)),
          field_0x58(static_cast<UnkMovedArray&&>(other.field_0x58)),
          field_0x80(static_cast<UnkMovedArray&&>(other.field_0x80)),
          field_0xa8(static_cast<UnkMovedArray&&>(other.field_0xa8)),
          field_0xd0(static_cast<UnkMovedArray&&>(other.field_0xd0)),
          field_0xf8(static_cast<UnkMovedArray&&>(other.field_0xf8)) {}
};

using UnkRecord120Table = base::global::CRntSmallDictionary<void*, UnkRecord120>;

void fn_7100b8a134(UnkRecord120Table* self, u32 n) {
    if (self->capacity >= n)
        return;
    UnkRecord120* records =
        static_cast<UnkRecord120*>(self->allocator->vf_0x40((u64)n * sizeof(UnkRecord120), 8, self->flags));
    void** ids = static_cast<void**>(self->allocator->vf_0x40((u64)n * sizeof(void*), 8, self->flags));
    if (UnkRecord120* old = self->values) {
        UnkRecord120* dst = records;
        for (u32 i = 0; i < self->count; ++i, ++old, ++dst) {
            new (dst) UnkRecord120(static_cast<UnkRecord120&&>(*old));
            fn_7100b8b7e8(old);
        }
        self->allocator->vf_0x48(self->values);
    }
    if (void** old = self->keys) {
        void** dst = ids;
        for (u32 i = 0; i < self->count; ++i, ++old, ++dst)
            __builtin_memcpy(dst, old, sizeof(void*));
        self->allocator->vf_0x48(self->keys);
    }
    self->values = records;
    self->keys = ids;
    self->capacity = n;
}
