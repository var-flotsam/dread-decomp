// Scales a six-float block by a factor that depends on how far the owner is from the position
// object along x: with the state word at +0x58 clear, the distance (absolute difference in x)
// divided by a range from the config at +0x50 gives a fraction clamped to [0, 1], which picks a
// value between the config's two limits. The block's x, y and z (at +0x60) are multiplied by it,
// and the result is stored twice in a row (+0x60..+0x74).
#include <math.h>

#include "game/UnkActor.h"
#include "types.h"

struct UnkConfig {
    u8 pad_0x0[0xf8];
    f32 field_0xf8;
    f32 field_0xfc;
    f32 field_0x100;
};

struct UnkScaled {
    u8 pad_0x0[0x60];
    f32 field_0x60;
    f32 field_0x64;
    f32 field_0x68;
    f32 field_0x6c;
    f32 field_0x70;
    f32 field_0x74;
};

struct UnkSelf {
    u8 pad_0x0[8];
    UnkActor* field_0x8;
    u8 pad_0x10[0x48];
    u32 field_0x58;
    u8 pad_0x5c[0x0];
};

struct UnkSelfCfg {
    u8 pad_0x0[0x50];
    UnkConfig* field_0x50;
};

// NON_MATCHING(minor): the z product has its operands in the other order (scheduling only)
void fn_7100437fb4(UnkSelf* self, UnkScaled* block) {
    if (self->field_0x58 != 0)
        return;
    f32 own_x = self->field_0x8->field_0x44;
    UnkActor* target = fn_7100d7c990(self->field_0x8);
    f32 distance = fabsf(own_x - target->field_0x44);
    UnkConfig* config = reinterpret_cast<UnkSelfCfg*>(self)->field_0x50;
    f32 fraction = distance / config->field_0x100;
    f32 t = fraction <= 0.0f ? 0.0f : (fraction >= 1.0f ? 1.0f : fraction);
    f32 factor = config->field_0xf8 + (config->field_0xfc - config->field_0xf8) * t;
    f32 x = block->field_0x60 * factor;
    f32 y = block->field_0x64 * factor;
    f32 z = block->field_0x68 * factor;
    block->field_0x60 = x;
    block->field_0x64 = y;
    block->field_0x68 = z;
    block->field_0x6c = x;
    block->field_0x70 = y;
    block->field_0x74 = z;
}
