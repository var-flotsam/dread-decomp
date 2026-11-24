#pragma once

#include "game/UnkFxObjectVtable_ovb0041.h"
#include "game/UnkXformNode.h"

struct UnkFxObject_ovb0041 {
    UnkFxObjectVtable_ovb0041* vtable;
    u8 pad_0x8[0x20];
    UnkXformNode field_0x28;  // the transform node, as CProjectileMovement_Burst_lac44a4.cpp has it
};
