#pragma once

// CLifeComponent: functions and globals of this translation unit that other files use (tools/tu_map.py unit 2715,
// 0x710115ddb4-0x7101161bb0). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkLifeComponentFull_ovb0000;
struct UnkLifeChange_ovb0000;
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
bool fn_7101161088(void* object);
void fn_710116179c(void* signal, UnkLifeChange_ovb0000* change);

// Takes (fn_7101160214) or adds (fn_71011603a4) `fraction` of the maximum life; whether the life changed.
struct UnkLifeValues_b49;
bool fn_7101160214(UnkLifeValues_b49* self, f32 fraction);
bool fn_71011603a4(UnkLifeValues_b49* self, f32 fraction);
void fn_710115fd10(UnkLifeComponentFull_ovb0000* self);
extern void* g_unk_7101cb3db0[];
