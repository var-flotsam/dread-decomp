#pragma once

// CTriggerComponent: functions and globals of this translation unit that other files use (tools/tu_map.py unit 1578,
// 0x7100a59a00-0x7100a653b8). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "game/UnkTUProperty.h"
#include "types.h"

struct UnkProperty;
struct UnkHandleList;
struct UnkTriggerEntry_b50a5;
struct UnkTriggerOwned_b56;
struct UnkTrigger_b50a5;
void fn_7100a632e8(UnkTriggerOwned_b56* self);
UnkHandleList* fn_7100a5e1dc();

// The original's unit defines it, so the compiler knows it doesn't keep `entry`'s address; noescape says so here,
// which lets a caller's last call be a tail call.
void fn_7100a5fdcc(UnkTrigger_b50a5* self, UnkTriggerEntry_b50a5** __attribute__((noescape)) entry);
extern UnkProperty g_unk_717274d6c8;
extern UnkTunable g_unk_717274ddd8;
