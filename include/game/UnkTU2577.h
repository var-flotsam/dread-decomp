#pragma once

// UnkTU2577: functions of this translation unit that other files call (tools/tu_map.py unit 2577,
// 0x7100edf654-0x7100ef3494). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct CSubAreaManager;
struct UnkIdSet;
struct UnkSubAreaChange_b190;
void fn_7100ee1548(void* self);
void fn_7100ee8044(CSubAreaManager* self, void* prev, void* cur, UnkIdSet* set);
bool fn_7100eef674(void*);
void fn_7100ef1608(void* set, const UnkSubAreaChange_b190* change);
