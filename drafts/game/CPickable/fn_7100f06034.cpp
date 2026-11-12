// CPickable unit: stores a position and moves each attached sub-object to a spread-out offset of it.
// Names and layouts are placeholders.
#include "types.h"

struct UnkStruct_71007455dc;
void fn_710074566c(UnkStruct_71007455dc* self, const f32* pos);

struct UnkPickable {
    u8 pad_0x00[0x20];
    UnkStruct_71007455dc* parts[2];
    u32 partCount;
    u8 pad_0x34[0x7c - 0x34];
    f32 pos[3];
};
static_assert(__builtin_offsetof(UnkPickable, partCount) == 0x30);
static_assert(__builtin_offsetof(UnkPickable, pos) == 0x7c);

// NON_MATCHING(minor): same instructions; the load of pos[2] and the multiply by zero are scheduled in a different order
void fn_7100f06034(UnkPickable* self, const f32* pos) {
    self->pos[0] = pos[0];
    self->pos[1] = pos[1];
    self->pos[2] = pos[2];
    if (self->partCount == 0) {
        return;
    }
    f32 zero = 0.0f;
    alignas(8) f32 out[3];
    for (u64 i = 0; i < self->partCount; i++) {
        f32 step = (f32)(u32)i * 0.1f;
        f32 spread = step * zero;
        f32 px = pos[0];
        f32 py = pos[1];
        f32 pz = pos[2];
        f32 z = step + pz;
        f32 x = spread + px;
        f32 y = spread + py;
        out[0] = x;
        out[1] = y;
        out[2] = z;
        fn_710074566c(self->parts[i], out);
    }
}
