#pragma once

// CHeatRoomComponent: functions and globals of this translation unit that other files use (tools/tu_map.py unit 1538,
// 0x7100a24a50-0x7100a287fc). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "game/UnkTUProperty.h"
#include "types.h"

struct UnkHeatRoom_b49;
void fn_7100a24a50(void* p);
void fn_7100a26828(UnkHeatRoom_b49* self);
extern void* g_unk_7101a8d830[];
extern char g_unk_7101a8d620[];
// "HeatRoomEnabled": the blackboard property the room's state is stored under (the qualified name its
// 0x160-byte member at +0x10 holds is built on first use; src/game/CHeatRoomComponent/CHeatRoomComponent_b49.cpp).
extern UnkPropertyEx<0x160> g_unk_7172746208;
