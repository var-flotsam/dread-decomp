#pragma once

// One object that 2 files viewed in part (fn_7100e8d8b0 returns it to both; views of 14040384 and 14040904 bytes whose shared offsets agree). Names and layouts are placeholders.
#include "types.h"
#include "base/global/CStrId.h"

struct UnkMapMarkers;
struct UnkMapEntry_ovb0203;

struct UnkMapData {
    u8 pad_0x0[0x68];
    UnkMapMarkers* markers;
    u8 pad_0x70[0x4c];
    f32 zoom;
    u8 pad_0xc0[0x4];
    f32 field_0xc4;
    u8 pad_0xc8[0x48];
    base::global::CStrId field_0x110;
    u8 field_0x118[1];
    u8 pad_0x119[0xd63c17];
    f32 bounds[4];
    u8 pad_0xd63d40[0x200];
    UnkMapEntry_ovb0203* field_0xd63f40;
};
