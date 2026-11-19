#pragma once

// CPopUpComposition: functions and globals of this translation unit that other files use (tools/tu_map.py unit 1708,
// 0x7100b5e364-0x7100b60d54). One declaration per symbol: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

struct UnkAmiiboCallback_laf0d38;
struct UnkAmiiboMessages_laf0d38;
struct UnkGuiComposition_ovb0090;
struct UnkMapCallback_ovb0090;
bool fn_7100b5fde0(UnkGuiComposition_ovb0090* gui, const base::global::CStrId* text, const base::global::CStrId* accept, const base::global::CStrId* back, const UnkMapCallback_ovb0090* on_accept, const UnkMapCallback_ovb0090* on_back, u32 mode, f32 time);
void fn_7100b5fd6c(void* object, base::global::UnkStringData** name, void* block, bool a, bool b);
extern u8* g_unk_71727653a8 __attribute__((visibility("hidden")));
void fn_7100b5fd9c(UnkAmiiboMessages_laf0d38* self, const base::global::CStrId& text, UnkAmiiboCallback_laf0d38 callback, bool a, bool b);
