#pragma once

#include "game/UnkSubareaRenderOut.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkFollowPlayer_b190;
struct UnkSubArea;

struct UnkSubAreaMgr_b190 {
    u8 pad_0x0[0x108];
    u32 field_0x108;
    u8 pad_0x10c[0x4];
    u32 field_0x110;
    u8 pad_0x114[0x8];
    UnkVec3f field_0x11c;
    UnkVec3f field_0x128;
    UnkVec3f field_0x134;
    u8 pad_0x140[0x4];
    u32 field_0x144;
    UnkFollowPlayer_b190* actor;  // 0x148
    bool active;                  // 0x150
    u8 pad_0x151[0x29f];
    UnkSubArea* field_0x3f0;
    UnkSubArea* field_0x3f8;
    u8 pad_0x400[0x538];
    UnkSubareaRenderOut field_0x938;
    u8 pad_0x948[0x18];
    UnkSubareaRenderOut field_0x960;
    u8 pad_0x970[0x18];
    f32 field_0x988;
    u8 pad_0x98c[0x154];
    bool field_0xae0;
    u8 pad_0xae1[0x7];
};
