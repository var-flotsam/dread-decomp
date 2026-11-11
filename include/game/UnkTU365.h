#pragma once

// UnkTU365: functions and globals of this translation unit that other files use (tools/tu_map.py unit 365,
// 0x7100288e2c-0x71002ac2f4). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkManager_7100863a8c;
struct UnkShapeTester_A5_b68;
struct UnkVisitResult_b0040;
struct UnkAssignedSlot;
struct UnkBreakablePart;
struct UnkBreakableTarget;
struct UnkTracker;
void fn_71002a5d1c(UnkAssignedSlot* self, const UnkAssignedSlot* other);
UnkBreakableTarget** fn_71002a76a4(UnkBreakablePart* part);
UnkTracker* fn_710028aca8(void* link);
extern UnkManager_7100863a8c* g_unk_71726a4788;  // The manager instance.
extern UnkVisitResult_b0040 g_unk_71726a4778;  // The "skip" result.
extern u64 g_unk_71726a4780;  // A guard.
extern UnkShapeTester_A5_b68* g_unk_71726a47a0;
extern void* g_unk_7101a20ed8[];
extern void* g_unk_7101a20fa8[];
extern char g_unk_7101a20f48[];
extern char g_unk_7101a20f68[];
extern char g_unk_7101a20f88[];
