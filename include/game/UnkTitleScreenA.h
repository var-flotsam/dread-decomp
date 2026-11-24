#pragma once

// One object that 2 files viewed in part (fn_7100b53b44 and fn_7100b53fa0 are declared with it in both; views of 64 and 48 bytes whose shared offsets agree). Names and layouts are placeholders.
#include "types.h"
#include "game/sound/UnkSoundHandle.h"

struct UnkGuiDisplayObject;

struct UnkTitleScreenA {
    void* vtable;
    UnkGuiDisplayObject* root;
    s32 state;
    u8 pad_0x14[0xc];
    f32 delay;
    bool done;
    bool notify;
    u8 pad_0x26[0x2];
    f32 field_0x28;
    u8 pad_0x2c[0x4];
    UnkSoundHandle sound;
};
