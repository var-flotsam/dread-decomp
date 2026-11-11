#pragma once

// CCharClassComponent: functions of this translation unit that other files call (tools/tu_map.py unit 2741,
// 0x71011aca90-0x71011ad580). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkIdObject;
struct UnkModelEntity;
struct UnkStruct_71008bca30;
void fn_71011acd10(UnkStruct_71008bca30*);  // Derived classes call it as their base's constructor.
void fn_71011acd74(UnkIdObject* self);  // Derived classes call it as their base's destructor.
bool fn_71011ace94(UnkModelEntity* self, void* arg);
