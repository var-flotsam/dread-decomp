#pragma once

// CPlayerLifeComponent: globals of this translation unit that other files use (tools/tu_map.py unit 2716,
// 0x7101161bb0-0x710116aa9c). One declaration per symbol: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkPlayerLife_ovb0049;
struct UnkGrabLife_A2_b71;
struct UnkGrabRequest_A2_b71;
struct UnkPlayerLifeTunable;
extern UnkPlayerLifeTunable g_unk_7172a0a0a8;  // CTunablePlayerLifeComponent (game/UnkPlayerLifeTunable.h)
void fn_7101166c6c(UnkGrabLife_A2_b71* self, UnkGrabRequest_A2_b71* request);
void fn_7101164cc0(UnkPlayerLife_ovb0049* self);
