#pragma once

// CCharClassLifeComponent: functions and globals of this translation unit that other files use (tools/tu_map.py unit 1368,
// 0x71008b4e90-0x71008bd078). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "game/UnkNamedString.h"
#include "types.h"

// The tunable g_unk_717272fd38: its group name and the floats its readers take.
struct UnkGroupIds {
    u8 unk_0x0[0x18];
    UnkNamedString* group;
    u8 unk_0x20[0x74];
    f32 field_0x94;
    u8 unk_0x98[0x154];
    f32 field_0x1ec;
};

void fn_71008bca30(void* self);
void fn_71008bcb3c(char* self);
extern UnkGroupIds g_unk_717272fd38;
