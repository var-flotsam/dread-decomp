#pragma once

// UnkTU22: functions of this translation unit that other files call (tools/tu_map.py unit 22,
// 0x710001449c-0x7100014d94). One declaration per symbol: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

struct UnkEnumInfo;
void fn_71000145e8(UnkEnumInfo* obj, base::global::CStrId* name, void (*fn)(), void* a);
