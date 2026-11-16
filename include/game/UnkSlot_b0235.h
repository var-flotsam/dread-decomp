#pragma once

// A resource slot of unit 210's GPU particle system (src/game/UnkTU210/UnkTU210_Emitter_ovb0235.cpp creates
// 0x400 of them): the resource and the slot's index in the slot table (-1 once released). Names and layout
// are placeholders.
#include "types.h"

struct UnkGfxBuffer252;

struct UnkSlot_b0235 {
    UnkGfxBuffer252* resource;
    s32 slot;  // 0x8
    u32 field_0xc;
};
