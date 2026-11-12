#pragma once

// CAutclastChargeAttack: functions and globals of this translation unit that other files use (tools/tu_map.py unit 555,
// 0x71003b4280-0x71003b8158). One declaration per symbol: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkFxObject_ovb0041;
struct UnkModelManager;

extern void* g_unk_7101a2aef8[];  // The listener's vtable.
extern void* g_unk_7101a2ae00[];  // The attack's vtable.
UnkFxObject_ovb0041* fn_71003b7db0(UnkModelManager* self, const void* path, bool a, bool b);
