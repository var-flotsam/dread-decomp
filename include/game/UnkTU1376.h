#pragma once

// UnkTU1376: functions and globals of this translation unit that other files use (tools/tu_map.py unit 1376,
// 0x71008cac70-0x71008db1d8). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

struct UnkStruct_71727313d4;
struct UnkStruct_71727313a4;
struct UnkProperty;
struct UnkDoorLife;
struct UnkDoorLifeA5_b61;
struct UnkDoorLife_b44a2;
struct UnkDoorLock;
struct UnkUnit2186Object;
struct UnkDoorShieldLife6_ovb0001;
bool OpenDoorUnlessOpen(void*);
void fn_71008d0a08(UnkDoorShieldLife6_ovb0001* self, const void* animation, bool reverse, bool loop);
void* fn_71008d89b0(void* owner);
void fn_71008d498c(UnkDoorLife_b44a2* self, u32 a, u32 b, u32 c);
UnkUnit2186Object* fn_71008d5d2c(void* owner);
void BreakDoorVignettesAndNotifyDoorOpened(UnkDoorLock* door);
void fn_71008d0c24(UnkDoorLifeA5_b61* self, f32 target, f32 speed);
bool IsPlayerOutsideDoorBounds(UnkDoorLife* self);
extern UnkProperty g_unk_7172731160;
extern UnkProperty g_unk_7172731110;  // "StayOpened".
extern UnkProperty g_unk_71727310c0;
extern UnkProperty g_unk_71727310e8;  // "Disabled".
extern UnkProperty g_unk_7172731138;  // "ChangeWhenVisible".
extern UnkProperty g_unk_7172731188;  // "ControlledByUsable".
bool fn_71008d586c(UnkDoorLock*, bool, bool, base::global::CStrId*);
bool fn_71008d564c(UnkDoorLock* door, u32 value, const base::global::CStrId* id);
extern UnkStruct_71727313a4 g_unk_71727313a4;
extern UnkStruct_71727313d4 g_unk_71727313d4;
