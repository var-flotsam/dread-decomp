#pragma once

// CPopUpComposition: functions of this translation unit that other files call (tools/tu_map.py unit 1708,
// 0x7100b5e364-0x7100b60d54). One declaration per symbol: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

struct UnkGuiComposition_ovb0090;
struct UnkMapCallback_ovb0090;
void fn_7100b5fde0(UnkGuiComposition_ovb0090* gui, const base::global::CStrId* text, const base::global::CStrId* accept, const base::global::CStrId* back, const UnkMapCallback_ovb0090* on_accept, const UnkMapCallback_ovb0090* on_back, u32 mode, f32 time);
void fn_7100b5fd6c(void* object, base::global::UnkStringData** name, void* block, bool a, bool b);
