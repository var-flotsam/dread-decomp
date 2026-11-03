// CSpawnPointComponent unit (tools/tu_map.py, unit 2704): the copy constructor of a base::UnkVector of
// 0x40-byte entries, the same code as fn_7100246a94 (src/game/CActor/CActor_VectorCopy_b41.cpp) with
// this unit's own entry move (fn_710111fd80) and append (fn_7101131b98): reset, copy the step, flags
// and allocator, reserve the other's capacity (entries moved, the old ones destroyed through their
// virtual destructor), then append a copy of each entry. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

// As src/clones/clones_06.cpp declares it: moves the entry at the second pointer into the first.
void fn_710111fd80(void*, void*);

struct UnkEntry_b42a3 {
    virtual ~UnkEntry_b42a3();
    u8 pad_0x8[0x38];
};

void fn_7101131b98(base::UnkVector<UnkEntry_b42a3>* self, const UnkEntry_b42a3* value);

void fn_7101131a84(base::UnkVector<UnkEntry_b42a3>* self, const base::UnkVector<UnkEntry_b42a3>* other) {
    self->data = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = other->step;
    self->field_0x14 = other->field_0x14;
    self->flags = other->flags;
    self->allocator = other->allocator;
    u32 n = other->capacity;
    if (self->capacity < n) {
        UnkEntry_b42a3* p = static_cast<UnkEntry_b42a3*>(self->allocator->vf_0x40((u64)n * sizeof(UnkEntry_b42a3), 8, self->flags));
        if (UnkEntry_b42a3* src = self->data) {
            UnkEntry_b42a3* dst = p;
            for (u32 i = 0; i < self->count; ++i) {
                fn_710111fd80(dst, src);
                (src++)->~UnkEntry_b42a3();
                ++dst;
            }
            self->allocator->vf_0x48(self->data);
        }
        self->data = p;
        self->capacity = n;
    }
    for (const UnkEntry_b42a3 *it = other->data, *end = other->data + other->count; it != end; ++it)
        fn_7101131b98(self, it);
}
