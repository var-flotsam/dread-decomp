#pragma once

// UnkTU2730: functions and globals of this translation unit that other files use (tools/tu_map.py unit 2730,
// 0x7101196284-0x710119acb0). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

struct UnkGameManagerNamedDef;
struct UnkCallback_kr;
bool fn_7101196fd8(void*);
bool fn_7101198688(void* gui);
void fn_71011995c0(void* owner, const UnkCallback_kr& callback);
void fn_7101199818(void* manager, void* component);
void fn_71011994e0(void* root, bool value);
void fn_7101199ac4(void* gui, void* target, const u32* mask, f32 a, f32 b);
extern UnkGameManagerNamedDef g_unk_7172a0bb10;  // The definition the cutscene's sub-id is read under.
void fn_71011986c8(void* manager, const base::global::CStrId* actor, bool visible, bool flag);
extern base::global::CStrId g_unk_7172a0bc98;
