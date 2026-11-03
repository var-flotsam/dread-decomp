// tools/tu_map.py unit 246 (beside src/base/spatial/UnkDecalProjector.cpp): a fade step: while the
// fade is on (+0x89) with a positive length (+0x80), counts the remaining time (+0x84) down by `dt`
// (not below 0) and hands the remaining fraction to the part at +0x78 (+0x70). Names are
// placeholders.
#include "types.h"

struct UnkFadeTarget246 {
    u8 pad_0x0[0x70];
    f32 fraction;  // 0x70
};

struct UnkFade246 {
    u8 pad_0x0[0x78];
    UnkFadeTarget246* target;  // 0x78
    f32 length;                // 0x80
    f32 remaining;             // 0x84
    u8 pad_0x88[0x1];
    bool active;  // 0x89
};

void fn_71001bba74(UnkFade246* self, f32 dt) {
    if (!self->active || !(self->length > 0.0f)) {
        return;
    }
    f32 remaining = self->remaining - dt;
    remaining = remaining < 0.0f ? 0.0f : remaining;
    self->remaining = remaining;
    self->target->fraction = remaining / self->length;
}
