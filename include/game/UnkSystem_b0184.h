#pragma once

#include "base/snd/UnkSoundSystemATK.h"
#include "game/UnkVoice_b0184.h"

// The members of the sound system these functions touch: two lists of voices and the mutex that
// guards them.
struct UnkSystem_b0184 {
    u8 pad_0x0[0xe8];
    u32 field_0xe8;
    u8 pad_0xec[0x3f0 - 0xec];
    UnkVoice_b0184** list_0x3f0;
    u32 count_0x3f8;
    u8 pad_0x3fc[0x1c];
    UnkVoice_b0184** list_0x418;
    u32 count_0x420;
    u8 pad_0x424[0x400568 - 0x424];
    nn::os::MutexType mutex;  // 0x400568
};
