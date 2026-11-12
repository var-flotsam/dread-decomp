#pragma once

// CCharClassBaseDamageTriggerComponent: functions of this translation unit that other files call (tools/tu_map.py unit 1561,
// 0x7100a45414-0x7100a461c0). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkLiquidPoolActor_ovb0075;
struct UnkLiquidPoolEnter_ovb0075;
struct UnkLiquidPoolDtor_ovb0075;
struct UnkLiquidPool_b44a2;
void fn_7100a45cac(UnkLiquidPool_b44a2* self);
void fn_7100a45a84(UnkLiquidPoolDtor_ovb0075* self);
void fn_7100a45f08(UnkLiquidPoolEnter_ovb0075* self, UnkLiquidPoolActor_ovb0075* actor, const UnkVec3f* pos);
