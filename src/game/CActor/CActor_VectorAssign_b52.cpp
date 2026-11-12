// Copy of fn_7101131d60 (src/game/CSpawnPointComponent/CSpawnPointComponent_b42.cpp) in the CActor
// unit: the copy assignment of a base::UnkVector of 0x40-byte entries beside fn_7100246a94
// (CActor_VectorCopy_b41.cpp): destroy the entries, reserve the other's capacity (entries moved with
// fn_71002458a4), then append a copy of each entry (fn_7100246ba8). Names and layouts are
// placeholders.
#include "base/UnkVector.h"
#include "game/UnkTU341.h"
#include "types.h"

// The move constructor and the entry, typed as CActor_VectorCopy_b41.cpp types them (one declaration
// per symbol).
struct UnkHalf_cu {
    u8 data[0x40];
    UnkHalf_cu(UnkHalf_cu&& other) asm("fn_71002458a4");
};

struct UnkEntry_b41a1 {
    virtual ~UnkEntry_b41a1();
    u8 pad_0x8[0x38];
};

base::UnkVector<UnkEntry_b41a1>* fn_7100246d70(base::UnkVector<UnkEntry_b41a1>* self,
                                            const base::UnkVector<UnkEntry_b41a1>* other) {
    if (self == other)
        return self;
    for (u64 i = 0; i < self->count; ++i)
        self->data[i].~UnkEntry_b41a1();
    self->count = 0;
    u32 n = other->capacity;
    if (self->capacity < n) {
        UnkEntry_b41a1* p = static_cast<UnkEntry_b41a1*>(self->allocator->vf_0x40((u64)n * sizeof(UnkEntry_b41a1), 8, self->flags));
        if (UnkEntry_b41a1* src_data = self->data) {
            UnkEntry_b41a1* d = p;
            for (u32 i = 0; i < self->count; ++i) {
                new (d) UnkHalf_cu(static_cast<UnkHalf_cu&&>(*reinterpret_cast<UnkHalf_cu*>(src_data)));
                (src_data++)->~UnkEntry_b41a1();
                ++d;
            }
            self->allocator->vf_0x48(self->data);
        }
        self->data = p;
        self->capacity = n;
    }
    for (const UnkEntry_b41a1 *it = other->data, *end = other->data + other->count; it != end; ++it)
        fn_7100246ba8(self, it);
    return self;
}
