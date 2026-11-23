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
    // Reflection's value operations (CType+0x28..+0x58), defined out of line.
    static void refl_CopyConstruct(CGroundShockerAttack* self, const CGroundShockerAttack* other);
    static void refl_MoveConstruct(CGroundShockerAttack* self, const CGroundShockerAttack* other);
    static CGroundShockerAttack* refl_CopyAssign(CGroundShockerAttack* self, const CGroundShockerAttack* other);
    static CGroundShockerAttack* refl_MoveAssign(CGroundShockerAttack* self, const CGroundShockerAttack* other);
};
