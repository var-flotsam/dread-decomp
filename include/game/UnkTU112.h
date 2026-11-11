#pragma once

// UnkTU112: functions of this translation unit that other files call (tools/tu_map.py unit 112,
// 0x71001395b8-0x7100140988). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkEntry_710013c3c8;
struct UnkTunableObject;
struct UnkTunableRegistry;
UnkTunableObject* fn_710013c5cc(UnkTunableRegistry* self, const char* name, bool qualified);
void fn_710013ffbc(UnkEntry_710013c3c8*, void*);
