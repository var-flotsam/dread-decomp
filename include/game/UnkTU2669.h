#pragma once

// UnkTU2669: functions of this translation unit that other files call (tools/tu_map.py unit 2669,
// 0x710109d670-0x710109e534). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "types.h"

struct UnkScriptManager_ovb0085;
struct UnkUnit2669Str;
void fn_710109d810(UnkScriptManager_ovb0085* self);
u32 fn_710109dd6c(void* self, void* source, UnkUnit2669Str* name, bool a, bool b);
// The script manager (0x88 bytes), created on first use.
void fn_710109d890(UnkScriptManager_ovb0085* self, base::global::CRntString* name);
