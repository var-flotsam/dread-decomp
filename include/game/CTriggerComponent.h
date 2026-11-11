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
void fn_7100a5fdcc(UnkTrigger_b50a5* self, UnkTriggerEntry_b50a5** __attribute__((noescape)) entry);
