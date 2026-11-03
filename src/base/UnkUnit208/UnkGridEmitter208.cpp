// Unit 208 of tools/tu_map.py (0x710016b51c-0x710016e03c): a particle emitter that places each
// particle on a cell of a grid picked by a sampler: index = sample * cell_count, cell =
// cells[index], position = origin - (cell % columns, cell / columns). Names and layouts are
// placeholders.
#include "types.h"

struct UnkVec2_208 {
    f32 x;
    f32 y;
};

struct UnkSource208 {
    u8 pad_0x0[0x38c];
    u32 particle_count;  // 0x38c
};

struct UnkTarget208 {
    u8 pad_0x0[0x8];
    UnkSource208* source;       // 0x8
    u8 pad_0x10[0x8];
    UnkVec2_208* positions;     // 0x18
};

class UnkSampler208 {
public:
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual f32 Sample(UnkSource208* source, u32 id);
};

struct UnkGridEmitter208 {
    void* vtable;
    UnkTarget208* target;    // 0x8
    UnkSampler208* sampler;  // 0x10
    u32 cell_count;          // 0x18
    u32 columns;             // 0x1c
    UnkVec2_208 origin;      // 0x20
    const u32* cells;        // 0x28
};

static inline void Place(UnkGridEmitter208* self, u32 id) {
    u32 index = (u32)(self->sampler->Sample(self->target->source, id) * (f32)self->cell_count);
    if (index < self->cell_count) {
        u32 cell = self->cells[index];
        u32 column = cell % self->columns;
        u32 row = cell / self->columns;
        f32 x = self->origin.x - (f32)column;
        f32 y = self->origin.y - (f32)row;
        UnkVec2_208& p = self->target->positions[id];
        p.x = x;
        p.y = y;
    }
}

void fn_710016d01c(UnkGridEmitter208* self, u32 id) {
    Place(self, id);
}

void fn_710016d0a4(UnkGridEmitter208* self, const u32* ids, u32 count) {
    for (; count != 0; --count, ++ids)
        Place(self, *ids);
}
