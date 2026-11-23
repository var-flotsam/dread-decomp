#pragma once

#include "game/UnkActor.h"
#include "game/UnkEmmyLabMark_A2_b70.h"

struct UnkEmmyLabAI_A2_b70 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x2a700 - 0x38];
    UnkEmmyLabMark_A2_b70 mark;  // 0x2a700
};
