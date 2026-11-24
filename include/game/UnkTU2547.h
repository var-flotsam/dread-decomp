#pragma once

// UnkTU2547: functions of this translation unit that other files call (tools/tu_map.py unit 2547,
// 0x7100ddb4f0-0x7100ddbcec). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkCentralUnit;
struct UnkDoorRegistry_b61;
struct UnkUnit2547;
void fn_7100ddb6dc(UnkDoorRegistry_b61* self);
UnkCentralUnit* GetFirstCentralUnitListObject(UnkUnit2547* self);
bool fn_7100ddb938(UnkDoorRegistry_b61* self);
void fn_7100ddbb7c(UnkUnit2547* self, u32 expected);
