#pragma once

// UnkTU2729: functions and globals of this translation unit that other files use (tools/tu_map.py unit 2729,
// 0x7101192c5c-0x7101196284). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkActorDefFactory;
struct UnkCache_71011934b0;
struct UnkCounted_71011934b0;
struct UnkGuiDef_b52;
struct UnkKey_71011934b0;
UnkGuiDef_b52* fn_7101193e88(UnkActorDefFactory*, const void*);
void fn_7101193cb4(UnkCache_71011934b0* self, UnkCounted_71011934b0* value, UnkKey_71011934b0* const* key);
extern u32 g_unk_7172a0ba48;
extern char g_unk_7101cb5270[];  // The factory's vtable (its second base's at +0x60).
