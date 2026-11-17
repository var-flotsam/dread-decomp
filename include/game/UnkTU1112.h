#pragma once

// UnkTU1112: functions and globals of this translation unit that other files use (tools/tu_map.py unit 1112,
// 0x71006f393c-0x71006f3e9c). One declaration per symbol: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/reflection/CClass.h"
#include "types.h"
#include "base/reflection/TypeObject.h"
enum class ENavigablePathType : u32;
template <> base::reflection::CType base::reflection::TypeObject<ENavigablePathType>::object asm("g_unk_717270c618");
