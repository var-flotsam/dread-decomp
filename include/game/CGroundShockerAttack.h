#pragma once

#include "game/CAttack.h"
#include "types.h"

struct CGroundShockerAttack : CAttack {
    ~CGroundShockerAttack() override;
    u64 field_0x28;
    u64 field_0x30;
    u64 field_0x38;
    UnkAttackListener field_0x40;
    __attribute__((always_inline)) CGroundShockerAttack& operator=(const CGroundShockerAttack&) = default;
};
