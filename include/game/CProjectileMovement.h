#pragma once

// CProjectileMovement: functions and globals of this translation unit that other files use (tools/tu_map.py unit 1623,
// 0x7100ac2430-0x7100ac6c44). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkChargeComponent;
struct UnkBossComponent;
struct UnkKraidBouncingCreaturesMovement2;
struct UnkKraidCreatureArg;
struct UnkNailongThornMovement;
struct UnkObj;
bool fn_7100ac4ec0(UnkObj* self, void* a1, void* a2, void* a3, void* a4, void* a5);
bool fn_7100ac3108(UnkKraidBouncingCreaturesMovement2* self, UnkKraidCreatureArg* arg);
bool fn_7100ac4c60(UnkBossComponent* self, u64 a, u64 b, u64 c, u64 d);
void fn_7100ac2c5c(void* self);
void fn_7100ac3a78(UnkNailongThornMovement*, f32);
void fn_7100ac38fc(void* self);  // Takes the component (it reads the rate at +0x7c itself).
extern void* g_unk_7101a96b30[];
void fn_7100ac40fc(UnkChargeComponent* self, f32 time);
