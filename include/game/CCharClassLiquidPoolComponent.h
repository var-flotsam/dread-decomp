#pragma once

// CCharClassLiquidPoolComponent: functions and globals of this translation unit that other files use (tools/tu_map.py unit 1394,
// 0x71008fdfb8-0x71009058b0). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/reflection/CClass.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkWaterPoolComponent_b0086;
struct UnkLiquidPoolEnter_ovb0075;
struct UnkLiquidPoolActor_ovb0075;
struct UnkLiquidPoolMover_ovb0075;
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
extern void* g_unk_7101a80ae8[];
extern base::reflection::CType g_unk_7172733a70;
void fn_7100900c44(UnkWaterPoolComponent_b0086* self);
void fn_71009014a8(UnkLiquidPoolEnter_ovb0075* self, UnkVec3f pos);
UnkLiquidPoolMover_ovb0075* fn_71009019bc(UnkLiquidPoolActor_ovb0075* actor);
