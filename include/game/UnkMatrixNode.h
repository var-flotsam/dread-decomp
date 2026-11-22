#pragma once

#include "game/UnkSphere.h"
#include "types.h"

struct UnkTarget;

struct UnkMatrixNode {
    u8 pad_0x0[0x24];
    f32 x_0x24;
    u8 pad_0x28[0xc];
    f32 y_0x34;
    u8 pad_0x38[0xc];
    f32 z_0x44;
    u8 pad_0x48[0x38];
    UnkSphere sphere_0x80;
    u8 pad_0xd0[0x2a];
    u8 dirty_0xfa;
    u8 pad_0xfb[0xd];
    UnkTarget* target_0x108;
};
