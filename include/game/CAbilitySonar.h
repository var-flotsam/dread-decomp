#pragma once

// CAbilitySonar: functions of this translation unit that other files call (tools/tu_map.py unit 434,
// 0x7100333544-0x710033a318). One declaration per symbol: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

struct UnkRenderPool;
struct UnkSwitchObject;
void* fn_7100339d48(UnkRenderPool* pool, const base::global::CFilePathStrId* path, UnkSwitchObject** out, const base::global::CStrId* name);
