#pragma once

#include "game/UnkGuiDisplayObject.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

struct UnkTitleScreenB {
    void* vtable;
    UnkGuiDisplayObject* root;  // 0x8
    u8 pad_0x10[0x4];
    u32 field_0x14;
    f32 delay;  // 0x18
    bool done;  // 0x1c
    u8 pad_0x1d[0x13];
    UnkSoundHandle sound;  // 0x30
};
