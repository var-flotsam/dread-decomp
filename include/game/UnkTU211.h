#pragma once

// UnkTU211: functions and globals of this translation unit that other files use (tools/tu_map.py unit 211,
// 0x7100190154-0x7100194a9c). One declaration per symbol: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/math/CVector4D.h"
#include "types.h"

struct UnkEntryListA1_134;
struct UnkDecalResourceMgr;
extern UnkDecalResourceMgr* g_unk_716e352450;
bool fn_7100193410(UnkEntryListA1_134* list, u32 index, u32 slot, const base::math::UnkVector4D& value, u32 flag);
