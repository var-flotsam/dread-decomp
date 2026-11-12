#pragma once

// CCharClassLiquidPoolComponent: functions of this translation unit that other files call (tools/tu_map.py unit 1394,
// 0x71008fdfb8-0x71009058b0). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkBaseComponent;
struct UnkPropertyArrays;
struct UnkLavaPool_b48;
struct UnkLiquidPoolLevel;
struct UnkLiquidPool_b44a2;
void fn_7100900928(UnkLiquidPoolLevel* self);
void fn_7100900cf4(UnkLiquidPool_b44a2* self);
void fn_7100901260(UnkLavaPool_b48* self);
f32 fn_7100902abc(void* a);
void fn_7100900414(UnkBaseComponent* self);
void fn_7100904c48(UnkPropertyArrays* self, u32 capacity);  // Reserve.
