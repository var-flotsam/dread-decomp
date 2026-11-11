#pragma once

// CLifeComponent: functions of this translation unit that other files call (tools/tu_map.py unit 2715,
// 0x710115ddb4-0x7101161bb0). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkCharacterLife_b0050;
struct UnkCommanderLife_A5_b63;
struct UnkHolder;
void fn_710115f7bc(void* self);
void fn_710115fc5c(char*);
void fn_710115fe20(UnkHolder* holder);
void fn_7101160078(UnkCommanderLife_A5_b63* self, f32 dt);
bool fn_7101160348(void* object, f32 duration);
void fn_710115f91c(void* self);
void fn_710115fe1c(char*);
bool fn_7101160408(UnkCharacterLife_b0050* self);
