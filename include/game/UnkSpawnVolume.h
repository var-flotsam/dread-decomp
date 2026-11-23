#pragma once

#include "game/UnkMat4f.h"

struct UnkSpawnVolume {
    u8 unk_0x0[0x1c];
    UnkMat4f matrix;
    u8 unk_0x5c[0xc];
    f32 size[3];
    u8 unk_0x74[0x18];
    s32 shape;
    f32 inner;
};
