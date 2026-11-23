#pragma once

#include "types.h"

// A scene item (the arrays hold pointers to them): its id first.
struct UnkSceneItem {
    s32 id;
    u8 pad_0x4[0xd4];
    f32 field_0xd8;
    f32 field_0xdc;
    f32 field_0xe0;
};
