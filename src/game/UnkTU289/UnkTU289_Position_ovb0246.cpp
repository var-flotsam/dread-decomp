// Unit 289: stores a new 3-float position into the cursor and its target when it moved by more than 1e-4. Names and layouts are placeholders.
#include "types.h"

struct UnkVec3_b0246 {
    f32 x, y, z;
};

struct UnkTarget_b0246 {
    u8 pad_0x0[0x88];
    f32 x, y, z;  // 0x88
};

struct UnkCursor_b0246 {
    u8 pad_0x0[0x24];
    f32 x;  // 0x24
    u8 pad_0x28[0xc];
    f32 y;  // 0x34
    u8 pad_0x38[0xc];
    f32 z;  // 0x44
    u8 pad_0x48[0xb2];
    u8 changed;  // 0xfa
    u8 applied;  // 0xfb
    u8 pad_0xfc[0xc];
    UnkTarget_b0246* target;  // 0x108
};

static inline bool Near(f32 a, f32 b) { return (a - 1e-4f <= b) && (a + 1e-4f >= b); }
void fn_710020bbe4(UnkCursor_b0246* self, UnkVec3_b0246* v) {
    if (Near(self->x, v->x) && Near(self->y, v->y) && Near(self->z, v->z))
        return;
    f32 z = v->z;
    self->y = v->y;
    self->x = v->x;
    self->changed = 1;
    self->z = z;
    // keeps the stores to the cursor ordered before the reads of the source for the target copy
    asm volatile("" ::: "memory");
    UnkTarget_b0246* t = self->target;
    t->x = v->x;
    t->y = v->y;
    t->z = v->z;
    self->applied = 1;
}
