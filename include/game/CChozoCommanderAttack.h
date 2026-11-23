#pragma once

#include "game/CAttack.h"

struct CChozoCommanderAttack : CAttack {
    ~CChozoCommanderAttack() override;
    u64 field_0x28;
    u64 field_0x30;
    u64 field_0x38;
    u64 field_0x40;
};
