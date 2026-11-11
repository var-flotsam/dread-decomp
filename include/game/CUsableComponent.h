#pragma once

// CUsableComponent: functions of this translation unit that other files call (tools/tu_map.py unit 1607,
// 0x7100a9bc70-0x7100aa4b74). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkShipRecharge_A5_b73;
struct UnkThermalDevice_b50a5;
struct UnkUsableComponent_A6_b62;
struct UnkUsableDialog_A6_b62;
struct UnkUsableVirt_A6_b62;
struct UnkUsable_b48;
void fn_7100a9e244(void*);  // Derived classes call it as their base's constructor.
void fn_7100a9f7f0(char* self);
bool fn_7100aa0b04(UnkUsableVirt_A6_b62* self, u64 a, u64 b, const u8* c);
bool fn_7100aa2948(UnkUsableComponent_A6_b62*);
void fn_7100a9e6bc(char*);
void fn_7100a9fad8(UnkThermalDevice_b50a5* self, f32 dt);
void fn_7100aa0034(void*);
u64 fn_7100aa1888(UnkShipRecharge_A5_b73* self, u32 state);
u64 fn_7100aa1f50(UnkShipRecharge_A5_b73* self, u32 state);
void fn_7100aa2570(UnkUsableDialog_A6_b62* self, bool show);
void fn_7100aa30bc(UnkUsable_b48* self, u32 arg);
