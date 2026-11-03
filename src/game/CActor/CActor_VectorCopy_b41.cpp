// CActor unit (tools/tu_map.py, hint from reflection): the copy constructor of a base::UnkVector of
// 0x40-byte entries, the shape of fn_710077a51c (CCentralUnitComponent): reset, copy the step, flags and
// allocator, reserve the other's capacity (entries moved with fn_71002458a4, the old ones destroyed
// through their virtual destructor), then append a copy of each entry (fn_7100246ba8). Names and
// layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

// The move constructor, typed as CCentralUnitComponent's 0x40-byte half (one declaration per symbol).
struct UnkHalf_cu {
    u8 data[0x40];
    UnkHalf_cu(UnkHalf_cu&& other) asm("fn_71002458a4");
};

struct UnkEntry_b41a1 {
    virtual ~UnkEntry_b41a1();
    u8 pad_0x8[0x38];
};

void fn_7100246ba8(base::UnkVector<UnkEntry_b41a1>* self, const UnkEntry_b41a1* value);

void fn_7100246a94(base::UnkVector<UnkEntry_b41a1>* self, const base::UnkVector<UnkEntry_b41a1>* other) {
    self->data = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = other->step;
    self->field_0x14 = other->field_0x14;
    self->flags = other->flags;
    self->allocator = other->allocator;
    u32 n = other->capacity;
    if (self->capacity < n) {
        UnkEntry_b41a1* p = static_cast<UnkEntry_b41a1*>(self->allocator->vf_0x40((u64)n * sizeof(UnkEntry_b41a1), 8, self->flags));
        if (UnkEntry_b41a1* src = self->data) {
            UnkEntry_b41a1* dst = p;
            for (u32 i = 0; i < self->count; ++i) {
                new (dst) UnkHalf_cu(static_cast<UnkHalf_cu&&>(*reinterpret_cast<UnkHalf_cu*>(src)));
                (src++)->~UnkEntry_b41a1();
                ++dst;
            }
            self->allocator->vf_0x48(self->data);
        }
        self->data = p;
        self->capacity = n;
    }
    for (const UnkEntry_b41a1 *it = other->data, *end = other->data + other->count; it != end; ++it)
        fn_7100246ba8(self, it);
}
