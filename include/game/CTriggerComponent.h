#pragma once

// CTriggerComponent: functions of this translation unit that other files call (tools/tu_map.py unit 1578,
// 0x7100a59a00-0x7100a653b8). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkHandleList;
struct UnkTriggerEntry_b50a5;
struct UnkTriggerOwned_b56;
struct UnkTrigger_b50a5;
void fn_7100a632e8(UnkTriggerOwned_b56* self);
UnkHandleList* fn_7100a5e1dc();

// The original's unit defines it, so the compiler knows it doesn't keep `entry`'s address; noescape says so here,
// which lets a caller's last call be a tail call.
void fn_7100a5fdcc(UnkTrigger_b50a5* self, UnkTriggerEntry_b50a5** __attribute__((noescape)) entry);
