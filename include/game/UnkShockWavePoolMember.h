#pragma once

#include "game/CSpitclawkAIComponent.h"

struct UnkShockWavePoolMember {
    u8 unk_0x0[0x228];
    ~UnkShockWavePoolMember() { fn_71006c9404(this); }
};
