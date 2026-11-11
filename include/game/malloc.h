#pragma once

// malloc: functions and globals of this translation unit that other files use (tools/tu_map.py unit 110,
// 0x71001259f4-0x7100132c04). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkCircle2D;
struct UnkContactPoint;
struct UnkStatsObject;
void fn_7100131f48(UnkStatsObject* self);
bool fn_710012f260(const UnkCircle2D* a, void* x5, const void* b, UnkContactPoint* out);
void fn_7100132aa0(bool on);
bool fn_710012dd70(const UnkCircle2D* a, void* x5, const UnkCircle2D& b, UnkContactPoint* out);
extern u64 g_unk_710b371030[2];  // The user id.
extern s32 g_unk_710b371068;  // The operation mode (src/engine/SystemStartup.cpp).
