#pragma once

#include "types.h"

struct UnkThermalValue_b0279;
struct UnkThermalLink_b0279;
struct UnkStringEntry_b0279;

struct UnkThermalDevice_b0279 {
    u8 pad_0x0[0x30];
    void* owner;  // 0x30
    u8 pad_0x38[0x498];
    UnkThermalValue_b0279* values;  // 0x4d0
    u64* keys;                      // 0x4d8
    u32 key_count;                  // 0x4e0
    u8 pad_0x4e4[0x64];
    UnkThermalLink_b0279* links;  // 0x548
    u32 link_count;               // 0x550
    u8 pad_0x554[0x24];
    UnkStringEntry_b0279* script_a;  // 0x578
    UnkStringEntry_b0279* script_b;  // 0x580
};
