#pragma once

// The layouts the two copy assignments of CScorpiusMovingPoisonousGasAttack_Assign_A3_b76.cpp and
// CScorpiusMovingPoisonousGasAttack_BaseAssign_A3_b76.cpp share. Names and layouts are placeholders.
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkScorpiusGas12_A3_b76 {
    u32 v[3];
};

struct UnkScorpiusGas16_A3_b76 {
    u64 v[2];
};

struct UnkScorpiusGas32_A3_b76 {
    u64 v[4];
};

struct UnkScorpiusGasWeakRef_A3_b76 {
    UnkWeakHandle* handle;
};

struct UnkScorpiusGasBase_A3_b76 {
    void** vtable;
    UnkScorpiusGas32_A3_b76 field_0x8;
    u64 field_0x28;
    u64 field_0x30;
    UnkScorpiusGasWeakRef_A3_b76 ref;  // 0x38
    UnkScorpiusGas16_A3_b76 field_0x40;
    u32 field_0x50;
    u32 field_0x54;
    u32 field_0x58;
    UnkScorpiusGas12_A3_b76 field_0x5c;
};

UnkScorpiusGasBase_A3_b76* fn_7100689410(UnkScorpiusGasBase_A3_b76* self, const UnkScorpiusGasBase_A3_b76* other);
