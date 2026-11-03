// CGoliathXBurstProjectionBombMovement unit (tools/tu_map.py): the constructor of an array of 12-byte
// values (base::UnkVector's layout) from `count` values at `values`: its step, flags and allocator (the
// default one, fn_7100083990, when none is given), room for all of them, then each appended through
// the out-of-line PushBack (fn_710058c768, as src/game_s7/fn_7100121f78.cpp declares it). Names and
// layouts are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "types.h"

struct UnkPushedVector;
struct UnkValues3;
void fn_710058c768(UnkPushedVector* self, const UnkValues3* value);  // PushBack

// Copied word by word (as src/game_s7/fn_7100121f78.cpp's UnkValues3).
struct UnkBombValue_b49 {
    u32 values[3];

    UnkBombValue_b49(const UnkBombValue_b49& other) {
        __builtin_memcpy(&values[0], &other.values[0], sizeof(u32));
        __builtin_memcpy(&values[1], &other.values[1], sizeof(u32));
        __builtin_memcpy(&values[2], &other.values[2], sizeof(u32));
    }
};

struct UnkBombVector_b49 {
    UnkBombValue_b49* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool grows;
    u32 flags;
    base::UnkAllocator* allocator;
};

void fn_710058beec(UnkBombVector_b49* self, const UnkBombValue_b49* values, u64 count, u32 step,
                   base::UnkAllocator* allocator, u32 flags) {
    self->step = step;
    self->flags = flags;
    self->data = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->grows = step != 0;
    if (!allocator)
        allocator = fn_7100083990();
    self->allocator = allocator;
    u32 n = (u32)count;
    if (self->capacity < n) {
        auto* p = static_cast<UnkBombValue_b49*>(allocator->vf_0x40((u64)n * sizeof(UnkBombValue_b49), 8, self->flags));
        if (self->data) {
            UnkBombValue_b49* src = self->data;
            UnkBombValue_b49* dst = p;
            for (u32 i = 0; i < self->count; ++i, ++dst, ++src)
                new (dst) UnkBombValue_b49(*src);
            self->allocator->vf_0x48(self->data);
        }
        self->data = p;
        self->capacity = n;
    }
    for (const UnkBombValue_b49 *it = values, *end = values + count; it != end; ++it) {
        UnkBombValue_b49 value = *it;
        fn_710058c768(reinterpret_cast<UnkPushedVector*>(self), reinterpret_cast<const UnkValues3*>(&value));
    }
}
