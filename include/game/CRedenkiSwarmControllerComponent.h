#pragma once

// CRedenkiSwarmControllerComponent: functions and globals of this translation unit that other files use (tools/tu_map.py unit 977,
// 0x710064eda0-0x710065b9f4). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkSwarmPath_b0012;
struct UnkZeroVec_b0012;
struct UnkRedenkiController_b55;
struct UnkTarget;
class UnkDaivoSwarm_b0012;
struct UnkSwarmHold_b66a4;
struct UnkVec3Result;
UnkVec3Result fn_710064fef8(UnkSwarmHold_b66a4* self);
bool fn_710065634c(UnkDaivoSwarm_b0012* self);
extern char g_unk_7101a5af00[];
void fn_7100655280(UnkRedenkiController_b55* self, UnkTarget* target);
void fn_7100656f3c(UnkDaivoSwarm_b0012* self, UnkSwarmPath_b0012* path, const UnkVec3Result* dir, const f32* speed, bool* out, const UnkZeroVec_b0012* offset, f32 radius, f32 limit);
